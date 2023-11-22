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
                const std::string mUpdateId;
                const std::string mFromId;
                const std::string mFromUserName;
                const std::string mFromName;
                const std::string mMessage;
            };

            //!
            void _SendMessage(const std::string chatId, const std::string message);

            //!
            // TelegramMessage _GetMessage();

            TelegramBot() {}
            ~TelegramBot() = default;
            TelegramBot(const TelegramBot&) = delete;
            TelegramBot& operator=(const TelegramBot&) = delete;

            static TelegramBot* mInstance;
            std::string mToken;
    };

} // namespace Subsystems

#endif // TELEGRAM_BOT_H