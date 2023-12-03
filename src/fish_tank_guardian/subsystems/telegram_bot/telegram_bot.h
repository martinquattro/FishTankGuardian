/*!****************************************************************************
 * @file    telegram_bot.h
 * @brief   Definition of the TelegramBot class.
 * @details This class provides integration with the Telegram API, allowing
 *          interaction and communication with users via the Telegram bot.
 * @author  Quattrone Martin
 * @date    Oct 2023
 *******************************************************************************/

#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H

#include <map>
#include <string>
#include <vector>
#include "delay.h"
#include "Json.h"
#include "mbed.h"
#include "real_time_clock.h"

#define BOT_API_URL                 "https://api.telegram.org/bot"
#define BOT_TOKEN                   "6738012692:AAFmeMoCUuZEGBGVwbxtFt8sC8f15o_aRgs"
#define USER_ID_EEPROM_START        7 * RTC_EEPROM_PAGE_SIZE

namespace Subsystems {

    class TelegramBot 
    {
        public:

            /**
             * @brief Initializes the TelegramBot subsystem.
             * @details This function initializes the TelegramBot subsystem and sets up
             * the necessary parameters for communication with the Telegram API.
             */
            static void Init();

            /**
             * @brief Gets the instance of the TelegramBot class.
             * @return Pointer to the TelegramBot instance.
             * @details This function returns a pointer to the singleton instance of
             * the TelegramBot class.
             */
            static TelegramBot* GetInstance();

            /**
             * @brief Updates the TelegramBot subsystem.
             * @details This function is responsible for updating the state of the
             * TelegramBot subsystem based on the current state and events.
             */
            void Update();

        private:

            /**
             * @brief Structure representing a Telegram message.
             * @details This structure holds information about a received Telegram message,
             * including the update ID, sender ID, sender username, sender name,
             * and the content of the message.
             */
            struct TelegramMessage 
            {
                unsigned long mUpdateId;
                std::string mFromId;
                std::string mFromUserName;
                std::string mFromName;
                std::string mMessage;
            };

            /**
             * @brief Enumeration representing the possible states of the TelegramBot.
             * @details The BOT_STATE enumeration defines the possible states that the
             * TelegramBot can be in during its operation.
             */
            enum class BOT_STATE 
            {
                INIT,                       //!< Initialization state.
                MONITOR,                    //!< Monitor state.
                SEND_ALERT,                 //!< Send alert state.
                REQUEST_LAST_MESSAGE,       //!< Request last message state.
                WAITING_LAST_MESSAGE,       //!< Waiting for last message state.
                PROCESS_LAST_MESSAGE,       //!< Process last message state.
                WAITING_RESPONSE,           //!< Waiting for response state.
            };

            using CommandFunction = std::function<std::string(const std::vector<std::string>&)>;

            /**
             * @brief Processes the "/start" command.
             * @param params The parameters associated with the command.
             * @return The response message for the command.
             * @details The _CommandStart function processes the "/start" command and
             * registers the user if the command is valid. It returns a response
             * message indicating the result of the registration.
             */
            std::string _CommandStart(const std::vector<std::string>& params);

            /**
             * @brief Processes the "/feeder_feed" command.
             * @param params The parameters associated with the command.
             * @return The response message for the command.
             * @details The _CommandFeederFeed function processes the "/feeder_feed"
             * command and triggers the food feeder to feed the specified
             * number of units. It returns a response message indicating
             * the result of the feeding operation.
             */
            std::string _CommandFeederFeed(const std::vector<std::string>& params);

            /**
             * @brief Processes the "/feeder_status" command.
             * @param params The parameters associated with the command.
             * @return The response message for the command.
             * @details The _CommandFeederStatus function processes the "/feeder_status"
             * command and retrieves the status of the food feeder, including
             * information about feed times and the number of units fed. It
             * returns a response message with the feeder status.
             */
            std::string _CommandFeederStatus(const std::vector<std::string>& params);

            /**
             * @brief Processes the "/feeder_set" command.
             * @param params The parameters associated with the command.
             * @return The response message for the command.
             * @details The _CommandFeederSet function processes the "/feeder_set"
             * command and adds a new feed time to the food feeder schedule.
             * It returns a response message indicating the result of the
             * feed time addition.
             */
            std::string _CommandFeederSet(const std::vector<std::string>& params);

            /**
             * @brief Processes the "/feeder_delete" command.
             * @param params The parameters associated with the command.
             * @return The response message for the command.
             * @details The _CommandFeederDelete function processes the "/feeder_delete"
             * command and deletes a feed time from the food feeder schedule.
             * It returns a response message indicating the result of the
             * feed time deletion.
             */
            std::string _CommandFeederDelete(const std::vector<std::string>& params);

            /**
             * @brief Processes the "/monitor_status" command.
             * @param params The parameters associated with the command.
             * @return The response message for the command.
             * @details The _CommandMonitorStatus function processes the "/monitor_status"
             * command and retrieves the status of the water monitor, including
             * temperature and TDS readings, as well as limits. It returns a
             * response message with the monitor status.
             */
            std::string _CommandMonitorStatus(const std::vector<std::string>& params);

            /**
            * @brief Set temperature limits for the monitoring system.
            * @param params Command parameters.
            * @return Response message.
            */
            std::string _CommandMonitorSetTempLimits(const std::vector<std::string>& params);

            /**
            * @brief Set TDS limits for the monitoring system.
            * @param params Command parameters.
            * @return Response message.
            */
            std::string _CommandMonitorSetTdsLimits(const std::vector<std::string>& params);

            /**
            * @brief Set the timezone for the system.
            * @param params Command parameters.
            * @return Response message.
            */
            std::string _CommandTimezone(const std::vector<std::string>& params);

            /**
            * @brief Get the current status of the water monitor and format it into a response message.
            * @return A string containing the formatted water monitor status.
            */
             std::string _GetMonitorStatusResponse();

            /**
            * @brief Register a user in the system using their user ID.
            * @param userId User ID to be registered.
            */
             void _RegisterUser(std::string userId);

            /**
            * @brief Check if a user is valid based on their user ID.
            * @param userId User ID to be checked.
            * @return True if the user is valid, false otherwise.
            */
             bool _IsValidUser(std::string userId);

            /**
            * @brief Get the user ID stored in the system.
            * @return The user ID stored in the system.
            */
             std::string _GetUserId();

            /**
            * @brief Parse a message into a vector of strings based on space separation.
            * @param message The message to be parsed.
            * @return A vector of strings containing the parsed message.
            */
             std::vector<std::string> _ParseMessage(const std::string& message);

            /**
            * @brief Send a message to a specified chat ID using the Telegram API.
            * @param chatId The chat ID to which the message will be sent.
            * @param message The message to be sent.
            */
             void _SendMessage(const std::string chatId, const std::string message);

            /**
            * @brief Request the last received message from the Telegram API.
            */
             void _RequestLastMessage();

            /**
            * @brief Extract and process a message from the Telegram API response.
            * @param message Pointer to a TelegramMessage object to store the extracted message.
            * @param response The response received from the Telegram API.
            * @return True if a new message is successfully extracted, false otherwise.
            */
             bool  _GetMessageFromResponse(TelegramMessage* messageconst, const std::string& response);

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
            std::map<std::string, CommandFunction> mCommandsMap;
    };

} // namespace Subsystems

#endif // TELEGRAM_BOT_H