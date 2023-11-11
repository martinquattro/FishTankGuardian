/*!****************************************************************************
 * @file telegram_bot.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H

#include "Json.h"

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

            TelegramBot() {}
            ~TelegramBot() = default;
            TelegramBot(const TelegramBot&) = delete;
            TelegramBot& operator=(const TelegramBot&) = delete;

            static TelegramBot* mInstance;
    };

} // namespace Subsystems

#endif // TELEGRAM_BOT_H