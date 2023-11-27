/*!****************************************************************************
 * @file telegram_bot.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H

#include "Json.h"
#include <string>
#include "mbed.h"
#include "delay.h"

#define BOT_API_URL     "https://api.telegram.org/bot"
#define BOT_TOKEN       "6738012692:AAFmeMoCUuZEGBGVwbxtFt8sC8f15o_aRgs"

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
                IDLE,
                REQUEST_LAST_MESSAGE,
                WAITING_LAST_MESSAGE,
                PROCESS_LAST_MESSAGE,
                WAITING_RESPONSE,
            };

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

    };

} // namespace Subsystems

#endif // TELEGRAM_BOT_H