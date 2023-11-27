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
#include "commands.h"

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
        case BOT_STATE::INIT:
        {
            mBotDelay.Start(DELAY_1_SECONDS);
            mState = BOT_STATE::IDLE;
        }
        break;

        case BOT_STATE::IDLE:
        {
            if (mBotDelay.HasFinished())
            {
                mState = BOT_STATE::REQUEST_LAST_MESSAGE;
            }
        }
        break;

        case BOT_STATE::REQUEST_LAST_MESSAGE:
        {
            if ((Drivers::WiFiCom::GetInstance()->IsBusy()) == false)
            {
                _RequestLastMessage();
                mState = BOT_STATE::WAITING_LAST_MESSAGE;
                mBotDelay.Start(DELAY_10_SECONDS);
            }
        }
        break;

        case BOT_STATE::WAITING_LAST_MESSAGE:
        {
            if (mBotDelay.HasFinished())
            {
                mState = BOT_STATE::IDLE;
            }
            else if (Drivers::WiFiCom::GetInstance()->GetResponse(&mResponse))
            {
                const bool isNewMessage = _GetMessageFromResponse(&mLastMessage, mResponse);
                if (isNewMessage)
                {
                    DEBUG_PRINT("TelegramBot - [OK] Message Obtained: [%s] from [%s]\r\n", mLastMessage.mMessage.c_str(), mLastMessage.mFromUserName.c_str());
                    mState = BOT_STATE::PROCESS_LAST_MESSAGE;
                }
                else
                {
                    mState = BOT_STATE::INIT;
                }
            }
        }
        break;

        case BOT_STATE::PROCESS_LAST_MESSAGE:
        {
            std::string messageToSend;

            std::vector<std::string> params = _ParseMessage(mLastMessage.mMessage);
            std::string command = params[0];

            if (mCommandsMap.find(command) != mCommandsMap.end()) 
            {
                // The corresponding command function is called
                CommandFunction commandFunction = mCommandsMap[command];
                messageToSend = commandFunction(params);
            }
            else
            {
                messageToSend = "Invalid command [" + command + "]";
            }

            // Every message received requires a response to the user
            _SendMessage(mLastMessage.mFromId, messageToSend);
            mState = BOT_STATE::WAITING_RESPONSE;
            mBotDelay.Start(DELAY_10_SECONDS);
        }
        break;

        case BOT_STATE::WAITING_RESPONSE:
        {
            const bool isResponseReady = Drivers::WiFiCom::GetInstance()->GetResponse(&mResponse);
            if ((mBotDelay.HasFinished()) || (isResponseReady && mResponse.compare(RESULT_ERROR) == 0))
            {
                mState = BOT_STATE::INIT;
                DEBUG_PRINT("TelegramBot - [ERROR] error in sent confirmation\r\n");
            }
            else if (Drivers::WiFiCom::GetInstance()->GetResponse(&mResponse))
            {
                mState = BOT_STATE::INIT;
                DEBUG_PRINT("TelegramBot - [OK] Sent confirmation Obtained: \r\n");
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
std::vector<std::string> TelegramBot::_ParseMessage(const std::string& message)
{
    size_t parameters_size = 0;
    int index_from = -1, index_to;
    const char separatorChar = ' ';

    if (message.length() > 0) 
    {
        parameters_size++;
    }

    while ((index_from = message.find_first_of(separatorChar, index_from + 1)) >= 0) 
    {
        parameters_size++;
    }

    std::vector<std::string> params(parameters_size);

    index_from = -1;
    for (size_t i = 0; i < parameters_size; i++) 
    {
        index_to = message.find_first_of(PARAM_SEPARATOR_CHAR, index_from + 1);
        
        if (index_to < 0)
        {
            index_to = message.length();
        }

        params[i] = message.substr(index_from + 1, index_to);
        index_from = index_to;
    }

    return params;
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
                    if (mLastUpdateId == 0)
                    {
                        // This is to avoid reading the last message that is in the conversation when rebooting or initiating
                        mLastUpdateId = stoul(updateId);
                        return false;
                    }

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
