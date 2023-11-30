/*!****************************************************************************
 * @file telegram_bot.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H

#include <map>
#include <string>
#include <vector>
#include "delay.h"
#include "Json.h"
#include "mbed.h"

#define BOT_API_URL     "https://api.telegram.org/bot"
#define BOT_TOKEN       "6738012692:AAFmeMoCUuZEGBGVwbxtFt8sC8f15o_aRgs"
#define USER_ID_EEPROM_START       224

namespace Subsystems { 

    class TelegramBot 
    {
        public:

            //! Initialize the subsystem
            static void Init();

            //! Returns the subsystem object
            static TelegramBot* GetInstance();

            //! Update subsystem
            void Update();

        private:

            using CommandFunction = std::function<std::string(const std::vector<std::string>&)>;

            struct TelegramMessage 
            {
                unsigned long mUpdateId;
                std::string mFromId;
                std::string mFromUserName;
                std::string mFromName;
                std::string mMessage;
            };

            enum class BOT_STATE
            {
                INIT,
                MONITOR,
                SEND_ALERT,
                REQUEST_LAST_MESSAGE,
                WAITING_LAST_MESSAGE,
                PROCESS_LAST_MESSAGE,
                WAITING_RESPONSE,
            };

            std::string _CommandStart(const std::vector<std::string>& params);
            std::string _CommandFeederFeed(const std::vector<std::string>& params);
            std::string _CommandFeederStatus(const std::vector<std::string>& params);
            std::string _CommandFeederSet(const std::vector<std::string>& params);
            std::string _CommandFeederDelete(const std::vector<std::string>& params);
            std::string _CommandMonitorStatus(const std::vector<std::string>& params);
            std::string _CommandMonitorSetTempLimits(const std::vector<std::string>& params);
            std::string _CommandMonitorSetTdsLimits(const std::vector<std::string>& params);
            std::string _CommandTimezone(const std::vector<std::string>& params);

            //!
            std::string _GetMonitorStatusResponse();

            //!
            void _RegisterUser(std::string userId);

            //!
            bool _IsValidUser(std::string userId);

            //! 
            std::string _GetUserId();

            //!
            std::vector<std::string> _ParseMessage(const std::string& message);

            //!
            void _SendMessage(const std::string chatId, const std::string message);

            //!
            void _RequestLastMessage();

            //!
            bool  _GetMessageFromResponse(TelegramMessage* messageconst, const std::string& response);

            //!
            TelegramMessage _GetLastMessage();

            TelegramBot(const char* apiUrl, const char* token);
            ~TelegramBot() = default;
            TelegramBot(const TelegramBot&) = delete;
            TelegramBot& operator=(const TelegramBot&) = delete;

            static TelegramBot* mInstance;
            BOT_STATE           mState;
            const std::string   mToken;
            const std::string   mBotUrl;
            unsigned long       mLastUpdateId;
            TelegramMessage     mLastMessage;
            std::string         mResponse;
            Util::Delay         mBotDelay;
            Util::Delay         mBotAlertsDelay;
            std::map<std::string, CommandFunction> mCommandsMap;
    };

} // namespace Subsystems

#endif // TELEGRAM_BOT_H