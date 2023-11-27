/*!****************************************************************************
 * @file telegram_bot.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "telegram_bot.h"

#include "arm_book_lib.h"
#include "wifi_com.h"

namespace Subsystems {

//=====[Declaration and initialization of private global variables]============

TelegramBot* TelegramBot::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void TelegramBot::Init()
{
    DEBUG_PRINT("TelegramBot - Initiating...\r\n");

    if (mInstance == nullptr)
    {
        mInstance = new TelegramBot(BOT_API_URL, BOT_TOKEN);
    }

    Drivers::WiFiCom::Init();

    DEBUG_PRINT("TelegramBot - [OK] Initialized\r\n");
}

//----static-------------------------------------------------------------------
TelegramBot* TelegramBot::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void TelegramBot::Update()
{
    switch (mState)
    {
        case BOT_STATE::IDLE:
        {
            mState = BOT_STATE::REQUEST_LAST_MESSAGE;
        }
        break;

        case BOT_STATE::REQUEST_LAST_MESSAGE:
        {
            if ((Drivers::WiFiCom::GetInstance()->IsBusy()) == false)
            {
                _RequestLastMessage();
                mState = BOT_STATE::WAITING_LAST_MESSAGE;
                mBotDelay.Start(DELAY_10_SECONDS);
                DEBUG_PRINT("TelegramBot - Requesting last message\r\n");
            }
        }
        break;

        case BOT_STATE::WAITING_LAST_MESSAGE:
        {
            if (mBotDelay.HasFinished())
            {
                mState = BOT_STATE::IDLE;
                DEBUG_PRINT("TelegramBot - [ERROR] receiving last message\r\n");
            }
            else if (Drivers::WiFiCom::GetInstance()->GetResponse(&mResponse))
            {
                const bool isValidMessage = _GetMessageFromResponse(&mLastMessage, mResponse);
                if (isValidMessage)
                {
                    DEBUG_PRINT("TelegramBot - [OK] Message Obtained: [%s] from [%s] fromId = [%s]\r\n", mLastMessage.mMessage.c_str(), mLastMessage.mFromUserName.c_str(), mLastMessage.mFromId.c_str());
                    mState = BOT_STATE::PROCESS_LAST_MESSAGE;
                }
                else
                {
                    mState = BOT_STATE::IDLE;
                }
            }
        }
        break;

        case BOT_STATE::PROCESS_LAST_MESSAGE:
        {
            std::string sendMessage = "Message Received : " + mLastMessage.mMessage;
            _SendMessage(mLastMessage.mFromId, sendMessage);
            mState = BOT_STATE::WAITING_RESPONSE;
            mBotDelay.Start(DELAY_10_SECONDS);
        }
        break;

        case BOT_STATE::WAITING_RESPONSE:
        {
            if ((mBotDelay.HasFinished()))
            {
                mState = BOT_STATE::IDLE;
                DEBUG_PRINT("TelegramBot - [ERROR] waiting for sent confirmation\r\n");
            }
            else if (Drivers::WiFiCom::GetInstance()->GetResponse(&mResponse))
            {
                mState = BOT_STATE::IDLE;
                DEBUG_PRINT("TelegramBot - [OK] Confirmation Obtained: [%s]\r\n", mResponse.c_str());
            }
        }
        break;
    }
}

//=====[Implementations of private functions]==================================

TelegramBot::TelegramBot(const char* apiUrl, const char* token)
    : mBotUrl(apiUrl)
    , mToken(token)
    , mBotDelay(0)
{
    mLastUpdateId = 0;
}

//-----------------------------------------------------------------------------
void TelegramBot::_SendMessage(const std::string chatId, const std::string message)
{
    std::string server = mBotUrl + mToken + "/sendmessage";
    std::string request = "chat_id=" + chatId + "&text=" + message;
    Drivers::WiFiCom::GetInstance()->Post(server, request);
}

//-----------------------------------------------------------------------------
bool TelegramBot::_GetMessageFromResponse(TelegramMessage* message, const std::string& response)
{
    Json json(response.c_str(), response.length(), 1000);

    if (json.isValidJson())
    {
        bool result = true;
        if (json.tokenBooleanValue(json.findChildIndexOf(json.findKeyIndex("ok")), result) == 0)
        {
            int keyIndexResultArray = json.findChildIndexOf(json.findKeyIndex("result"));
            int resultCount = json.childCount(keyIndexResultArray);

            if (resultCount > 0)
            {
                int keyResult = json.findChildIndexOf(keyIndexResultArray, 0);
                std::string updateId = json.tokenString(json.findChildIndexOf(json.findKeyIndexIn("update_id", keyResult)));

                if (updateId.compare("") && stoul(updateId) > mLastUpdateId)
                {
                    mLastUpdateId = stoul(updateId);

                    int keyMessage = json.findKeyIndexIn("message", keyResult);
                    int keyFrom = json.findKeyIndexIn("from", json.findChildIndexOf(keyMessage));
                    int keyFromId = json.findChildIndexOf(json.findKeyIndexIn("id", json.findChildIndexOf(keyFrom)));
                    int keyText = json.findChildIndexOf(json.findKeyIndexIn("text", json.findChildIndexOf(keyMessage)));
                    int keyFromName = json.findChildIndexOf(json.findKeyIndexIn("first_name", json.findChildIndexOf(keyFrom)));
                    int keyFromUsername = json.findChildIndexOf(json.findKeyIndexIn("username", json.findChildIndexOf(keyFrom)));

                    message->mUpdateId = mLastUpdateId;
                    message->mFromId = json.tokenString(keyFromId);
                    message->mFromName = json.tokenString(keyFromName);
                    message->mFromUserName = (keyFromUsername >= 0) ? json.tokenString(keyFromUsername) : message->mFromId;
                    message->mMessage = json.tokenString(keyText);
    
                    return true;
                }
            }
        }
    }

    return false;
}

//-----------------------------------------------------------------------------
void TelegramBot::_RequestLastMessage()
{
    std::string server = mBotUrl + mToken + "/getUpdates";
    std::string request = "offset=-1";
    Drivers::WiFiCom::GetInstance()->Post(server, request);
}

} // namespace Subsystems
