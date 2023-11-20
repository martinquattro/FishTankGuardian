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
        mInstance = new TelegramBot();
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
    Drivers::WiFiCom::GetInstance()->Update();
}

//=====[Implementations of private functions]==================================

//-----------------------------------------------------------------------------
void TelegramBot::_SendMessage(const std::string chatId, const std::string message)
{

}

//-----------------------------------------------------------------------------
// TelegramBot::TelegramMessage TelegramBot::_GetMessage()
// {
// //     TelegramMessage message;
// //     std::string server = "https://api.telegram.org/bot" + mToken + "/getUpdates";
// //     std::string response = "";//wifi.post(server, request);

// //     Json json(response.c_str(), response.length());

// //     if (!json.isValidJson())
// //     {
// //         return message;
// //     }

// //     bool result = true;

// //     if (json.tokenBooleanValue(json.findChildIndexOf(json.findKeyIndex("ok")), result) < 0)
// //     {
// //         return messages;
// //     }
// //     messages = parseJsonMessages(json);


// //   return message;

// //     return TelegramMessage();
// }

} // namespace Subsystems
