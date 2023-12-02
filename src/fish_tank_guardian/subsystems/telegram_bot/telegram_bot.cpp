/*!****************************************************************************
 * @file telegram_bot.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "telegram_bot.h"
#include "telegram_bot_lib.h"

#include <functional>
#include "arm_book_lib.h"
#include "commands.h"
#include "food_feeder.h"
#include "real_time_clock.h"
#include "utilities.h"
#include "water_monitor.h"
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

    mLastUpdateId = 0;

    mCommandsMap[COMMAND_START]                      = std::bind(&TelegramBot::_CommandStart, this, std::placeholders::_1);
    mCommandsMap[COMMAND_FEEDER_FEED]                = std::bind(&TelegramBot::_CommandFeederFeed, this, std::placeholders::_1);
    mCommandsMap[COMMAND_FEEDER_STATUS]              = std::bind(&TelegramBot::_CommandFeederStatus, this, std::placeholders::_1);
    mCommandsMap[COMMAND_FEEDER_SET]                 = std::bind(&TelegramBot::_CommandFeederSet, this, std::placeholders::_1);
    mCommandsMap[COMMAND_FEEDER_DELETE]              = std::bind(&TelegramBot::_CommandFeederDelete, this, std::placeholders::_1);
    mCommandsMap[COMMAND_TIMEZONE]                   = std::bind(&TelegramBot::_CommandTimezone, this, std::placeholders::_1);
    mCommandsMap[COMMAND_MONITOR_STATUS]             = std::bind(&TelegramBot::_CommandMonitorStatus, this, std::placeholders::_1);
    mCommandsMap[COMMAND_MONITOR_SET_TEMP_LIMITS]    = std::bind(&TelegramBot::_CommandMonitorSetTempLimits, this, std::placeholders::_1);
    mCommandsMap[COMMAND_MONITOR_SET_TDS_LIMITS]     = std::bind(&TelegramBot::_CommandMonitorSetTdsLimits, this, std::placeholders::_1);

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
            if (!(Drivers::WiFiCom::GetInstance()->IsBusy()) && (Subsystems::RealTimeClock::GetInstance()->IsReady()))
            {
                mState = BOT_STATE::MONITOR;
            }
        }
        break;

        case BOT_STATE::MONITOR:
        {
            if (mBotAlertsDelay.HasFinished() && !(Subsystems::WaterMonitor::GetInstance()->GetWaterState()))
            {
                mState = BOT_STATE::SEND_ALERT;
            }
            else
            {
                mState = BOT_STATE::REQUEST_LAST_MESSAGE;
                mBotDelay.Start(DELAY_2_SECONDS);
            }
        }
        break;
        
        case BOT_STATE::SEND_ALERT:
        {
            if (mBotAlertsDelay.HasFinished() && !(Drivers::WiFiCom::GetInstance()->IsBusy()))
            {
                std::string messageToSend = ALERT_OUT_OF_LIMITS;
                messageToSend += "\n";
                messageToSend += _GetMonitorStatusResponse();
                std::string userId = _GetUserId();
                _SendMessage(userId, messageToSend);
                mState = BOT_STATE::WAITING_RESPONSE;
                mBotDelay.Start(DELAY_5_SECONDS);
                mBotAlertsDelay.Start(DELAY_10_SECONDS);
            }
        }
        break;

        case BOT_STATE::REQUEST_LAST_MESSAGE:
        {
            if (mBotDelay.HasFinished() && !(Drivers::WiFiCom::GetInstance()->IsBusy()))
            {
                _RequestLastMessage();
                mState = BOT_STATE::WAITING_LAST_MESSAGE;
                mBotDelay.Start(DELAY_5_SECONDS);
            }
        }
        break;

        case BOT_STATE::WAITING_LAST_MESSAGE:
        {
            if (mBotDelay.HasFinished())
            {
                mState = BOT_STATE::INIT;
            }
            else if (Drivers::WiFiCom::GetInstance()->GetPostResponse(&mResponse))
            {
                const bool isNewMessage = _GetMessageFromResponse(&mLastMessage, mResponse);
                if (isNewMessage)
                {
                    DEBUG_PRINT("TelegramBot - [OK] Message Obtained: [%s] from [%s]\r\n", mLastMessage.mMessage.c_str(), mLastMessage.mFromName.c_str());
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

            if ((_IsValidUser(mLastMessage.mFromId)) || (command.compare(COMMAND_START) == 0))
            {
                if (mCommandsMap.find(command) != mCommandsMap.end()) 
                {
                    // The corresponding command function is called
                    CommandFunction commandFunction = mCommandsMap[command];
                    messageToSend = commandFunction(params);
                }
                else
                {
                    messageToSend = Utilities::FormatString(ERROR_INVALID_COMMAND, command.c_str());
                }
            }
            else
            {
                messageToSend = ERROR_INVALID_USER;
            }

            // Every message received requires a response to the user
            _SendMessage(mLastMessage.mFromId, messageToSend);
            mState = BOT_STATE::WAITING_RESPONSE;
            mBotDelay.Start(DELAY_5_SECONDS);
        }
        break;

        case BOT_STATE::WAITING_RESPONSE:
        {
            const bool isResponseReady = Drivers::WiFiCom::GetInstance()->GetPostResponse(&mResponse);
            if ((mBotDelay.HasFinished()) || (isResponseReady && mResponse.compare(RESULT_ERROR) == 0))
            {
                mState = BOT_STATE::INIT;
            }
            else if (Drivers::WiFiCom::GetInstance()->GetPostResponse(&mResponse))
            {
                mState = BOT_STATE::INIT;
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
    , mBotAlertsDelay(0)
{}

//-----------------------------------------------------------------------------
std::string TelegramBot::_CommandStart(const std::vector<std::string>& params)
{
    if (params.size() == 1) 
    {
        _RegisterUser(mLastMessage.mFromId);
        return (Utilities::FormatString("User registered correctly\nHi %s, I'm the Guardian\r\n", mLastMessage.mFromName.c_str()));
    }

    return (Utilities::FormatString(ERROR_INVALID_PARAMETERS, COMMAND_START));
}


//-----------------------------------------------------------------------------
std::string TelegramBot::_CommandFeederFeed(const std::vector<std::string>& params) 
{
    if (params.size() == 2) 
    {
        std::string numFeedsParam = params[1];

        if (Utilities::IsNumeric(numFeedsParam))
        {
            int feedNumber = std::stoi(numFeedsParam);

            if (Subsystems::FoodFeeder::GetInstance()->Feed(feedNumber))
            {
                return (Utilities::FormatString("[OK]\nI feeded [%d] units of food.", feedNumber));
            }
        }
    }
    
    return (Utilities::FormatString(ERROR_INVALID_PARAMETERS, COMMAND_FEEDER_FEED));
}

//-----------------------------------------------------------------------------
std::string TelegramBot::_CommandFeederStatus(const std::vector<std::string>& params)
{
    if (params.size() == 1) 
    {
        std::vector<FoodFeeder::FeedTimeInfo> feedTimesInfoList = Subsystems::FoodFeeder::GetInstance()->GetFeedTimes();

        if (feedTimesInfoList.empty()) 
        {
            return "[OK]\nNo feeding times available.";
        }

        std::string response = "[OK]\nFeeding Times:\n";
        for (const auto& feedTimeInfo : feedTimesInfoList) 
        {
            response += Utilities::FormatString("Slot %d - %s with %d food units\n",
                                                feedTimeInfo.mSlot,
                                                feedTimeInfo.mFeedTime.c_str(),
                                                feedTimeInfo.mNumFeeds);
        }

        return response;
    }

    return (Utilities::FormatString(ERROR_INVALID_PARAMETERS, COMMAND_FEEDER_STATUS));
}

//-----------------------------------------------------------------------------
std::string TelegramBot::_CommandFeederSet(const std::vector<std::string>& params)
{
    if (params.size() == 4) 
    {
        std::string slotParam = params[1];
        std::string timeParam = params[2];
        std::string numFeedsParam = params[3];

        if (Utilities::IsNumeric(slotParam) && Utilities::IsNumeric(numFeedsParam))
        {
            int slot = std::stoi(slotParam);
            int numFeeds = std::stoi(numFeedsParam);

            if (Subsystems::FoodFeeder::GetInstance()->AddFeedTime(timeParam, numFeeds, slot))
            {
                return Utilities::FormatString("[OK]\nFeed time added:\n"
                                               "Slot [%d] - [%s] with [%d] food units\n",
                                               slot, timeParam.c_str(), numFeeds);
            }
        }
    }

    return (Utilities::FormatString(ERROR_INVALID_PARAMETERS, COMMAND_FEEDER_SET));
}

//-----------------------------------------------------------------------------
std::string TelegramBot::_CommandFeederDelete(const std::vector<std::string>& params)
{
    if (params.size() == 2) 
    {
        std::string slotToDelete = params[1];

        if (Utilities::IsNumeric(slotToDelete)) 
        {
            int slot = std::stoi(slotToDelete);

            if (Subsystems::FoodFeeder::GetInstance()->EraseFeedTime(slot)) 
            {
                return (Utilities::FormatString("[OK]\nFeeder time slot [%d] deleted correctly", slot));
            }
        }
    }
    
    return (Utilities::FormatString(ERROR_INVALID_PARAMETERS, COMMAND_FEEDER_DELETE));
}

//-----------------------------------------------------------------------------
std::string TelegramBot::_CommandMonitorStatus(const std::vector<std::string>& params)
{
    if (params.size() == 1) 
    {
        return _GetMonitorStatusResponse();
    }

    return Utilities::FormatString(ERROR_INVALID_PARAMETERS, COMMAND_MONITOR_STATUS);
}

//-----------------------------------------------------------------------------
std::string TelegramBot::_CommandMonitorSetTempLimits(const std::vector<std::string>& params)
{
    if (params.size() == 3) 
    {
        std::string lowerLimitStr = params[1];
        std::string upperLimitStr = params[2];

        if (Utilities::IsNumeric(lowerLimitStr) && Utilities::IsNumeric(upperLimitStr))
        {
            int lowerLimit = std::stoi(lowerLimitStr);
            int upperLimit = std::stoi(upperLimitStr);

            if (Subsystems::WaterMonitor::GetInstance()->SetTemperatureLimits(lowerLimit, upperLimit))
            {
                return Utilities::FormatString("[OK]\nTemperature Limits [%d - %d] set correctly", lowerLimit, upperLimit);
            }
        }
    }

    return Utilities::FormatString(ERROR_INVALID_PARAMETERS, COMMAND_MONITOR_SET_TEMP_LIMITS);
}

//-----------------------------------------------------------------------------
std::string TelegramBot::_CommandMonitorSetTdsLimits(const std::vector<std::string>& params)
{
    if (params.size() == 3) 
    {
        std::string lowerLimitStr = params[1];
        std::string upperLimitStr = params[2];

        if (Utilities::IsNumeric(lowerLimitStr) && Utilities::IsNumeric(upperLimitStr))
        {
            int lowerLimit = std::stoi(lowerLimitStr);
            int upperLimit = std::stoi(upperLimitStr);

            if (Subsystems::WaterMonitor::GetInstance()->SetTdsLimits(lowerLimit, upperLimit))
            {
                return Utilities::FormatString("[OK]\nTds Limits [%d - %d] set correctly", lowerLimit, upperLimit);
            }
        }
    }

    return Utilities::FormatString(ERROR_INVALID_PARAMETERS, COMMAND_MONITOR_SET_TDS_LIMITS);
}

//-----------------------------------------------------------------------------
std::string TelegramBot::_CommandTimezone(const std::vector<std::string>& params)
{
    if (params.size() == 2) 
    {
        std::string timezone = params[1];

        if (Subsystems::RealTimeClock::GetInstance()->SetTimeZone(timezone))
        {
            return "[OK]\nValid Time Zone.\nSyncing...";
        }
    }

    return Utilities::FormatString(ERROR_INVALID_PARAMETERS, COMMAND_TIMEZONE);
}

//-----------------------------------------------------------------------------
std::string TelegramBot::_GetMonitorStatusResponse()
{
    int tempLowerLimit, tempUpperLimit, tdsLowerLimit, tdsUpperLimit;
    Subsystems::WaterMonitor::GetInstance()->GetTemperatureLimits(&tempLowerLimit, &tempUpperLimit);
    Subsystems::WaterMonitor::GetInstance()->GetTdsLimits(&tdsLowerLimit, &tdsUpperLimit);

    const float temp = Subsystems::WaterMonitor::GetInstance()->GetTempReading();
    const int tds = Subsystems::WaterMonitor::GetInstance()->GetTdsReading();

    return Utilities::FormatString( "- Temperature: [%.1f°C]\n"
                                    "- Temperature Limits: [%d°C - %d°C]\n"
                                    "- Tds Reading: [%03d PPM]\n"
                                    "- Tds Limits: [%03d PPM - %03d PPM]",
                                    temp, tempLowerLimit, tempUpperLimit,
                                    tds, tdsLowerLimit, tdsUpperLimit);
}

//-----------------------------------------------------------------------------
void TelegramBot::_RegisterUser(std::string userId)
{
    Subsystems::RealTimeClock::GetInstance()->SaveStringToEeprom(USER_ID_EEPROM_START, userId);
}

//-----------------------------------------------------------------------------
bool TelegramBot::_IsValidUser(std::string userId)
{
    std::string userIdInEeprom = Subsystems::RealTimeClock::GetInstance()->ReadStringFromEeprom(USER_ID_EEPROM_START);

    return (userIdInEeprom.compare(userId) == 0);
}

//-----------------------------------------------------------------------------
std::string TelegramBot::_GetUserId()
{
    return (Subsystems::RealTimeClock::GetInstance()->ReadStringFromEeprom(USER_ID_EEPROM_START));
}

//-----------------------------------------------------------------------------
std::vector<std::string> TelegramBot::_ParseMessage(const std::string& message)
{
    std::vector<std::string> params;
    size_t start = 0;
    size_t end = message.find(' ');

    while (end != std::string::npos)
    {
        params.push_back(message.substr(start, end - start));
        start = end + 1;
        end = message.find(' ', start);
    }

    // Agregar el último parámetro (si existe)
    if (start < message.length())
    {
        params.push_back(message.substr(start));
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
