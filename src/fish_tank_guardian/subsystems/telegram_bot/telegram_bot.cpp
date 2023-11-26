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
    DEBUG_PRINT("TelegramBot::Init() - Initiating...\r\n");

    if (mInstance == nullptr)
    {
        mInstance = new TelegramBot(BOT_API_URL, BOT_TOKEN);
    }

    Drivers::WiFiCom::Init();

    DEBUG_PRINT("TelegramBot::Init() - Initiating Finished.\r\n");
}

//----static-------------------------------------------------------------------
TelegramBot* TelegramBot::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void TelegramBot::Update()
{
    TelegramMessage message;

    Drivers::WiFiCom::GetInstance()->Update();

    if ((Drivers::WiFiCom::GetInstance()->IsBusy()) == false)
    {
        // DEBUG_PRINT("TelegramBot::Update() - WiFi not busy\r\n");
        _RequestLastMessage();
    }
    else if (_IsLastMessageReady(&message))
    {
        DEBUG_PRINT("TelegramBot::Update() - Message Obtained: mUpdateId = %lu - mFromId = %s - mFromName = %s - mFromUserName = %s - mMessage = %s\r\n", 
            message.mUpdateId,
            message.mFromId.c_str(),
            message.mFromName.c_str(),
            message.mFromUserName.c_str(),
            message.mMessage.c_str()
        );
    }
}

//=====[Implementations of private functions]==================================

TelegramBot::TelegramBot(const char* apiUrl, const char* token)
    : mBotUrl(apiUrl)
    , mToken(token)
{
    mLastUpdateId = 0;
}

//-----------------------------------------------------------------------------
void TelegramBot::_SendMessage(const std::string chatId, const std::string message)
{

}

//-----------------------------------------------------------------------------
bool TelegramBot::_IsLastMessageReady(TelegramMessage* message)
{
    bool isReady = false;

    if (Drivers::WiFiCom::GetInstance()->IsPostResponseReady())
    {
        std::string response = Drivers::WiFiCom::GetInstance()->GetResponse();
        isReady = _GetMessageFromResponse(message, response);
    }

    return isReady;
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
                unsigned long numberUpdateId = stoul(updateId);

                if (numberUpdateId > mLastUpdateId)
                {
                    mLastUpdateId = numberUpdateId;

                    int keyMessage = json.findKeyIndexIn("message", keyResult);
                    int keyFrom = json.findKeyIndexIn("from", json.findChildIndexOf(keyMessage));
                    int keyFromId = json.findChildIndexOf(json.findKeyIndexIn("id", json.findChildIndexOf(keyFrom)));
                    int keyText = json.findChildIndexOf(json.findKeyIndexIn("text", json.findChildIndexOf(keyMessage)));
                    int keyFromName = json.findChildIndexOf(json.findKeyIndexIn("first_name", json.findChildIndexOf(keyFrom)));
                    int keyFromUsername = json.findChildIndexOf(json.findKeyIndexIn("username", json.findChildIndexOf(keyFrom)));

                    message->mUpdateId = numberUpdateId;
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
