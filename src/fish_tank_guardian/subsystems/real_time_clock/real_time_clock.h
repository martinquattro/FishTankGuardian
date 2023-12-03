/*!****************************************************************************
 * @file    real_time_clock.h
 * @brief   RealTimeClock class header file.
 * @author  Quattrone Martin
 * @date    Oct 2023
 ******************************************************************************/

#ifndef REAL_TIME_CLOCK_H
#define REAL_TIME_CLOCK_H

#include "mbed.h"
#include "memory.h"
#include <string>
#include "delay.h"
#include <map>

#define RTC_PIN_SDA                 PB_11
#define RTC_PIN_SCL                 PB_10
#define RTC_ADDRESS_ID              0x68
#define RTC_EEPROM_ADDRESS          0x50
#define RTC_EEPROM_PAGE_SIZE        32
#define RTC_GET_TIME_URL            "https://www.timeapi.io/api/Time/current/zone?timeZone="
#define MAX_SYNC_ATTEMPTS           5
#define RTC_TIMEZONE_EEPROM_START   10 * RTC_EEPROM_PAGE_SIZE

namespace Subsystems {

    class RealTimeClock 
    {
        public:

            //! Initializes the RTC.
            static void Init();

            /*!
            * @brief Gets the singleton instance of the RTC.
            * @return RealTimeClock* Pointer to the RealTimeClock instance.
            */
            static RealTimeClock* GetInstance();

            //! Updates the RTC.
            void Update();

            /*!
            * @brief Checks if the RTC is ready.
            * @return True if ready (synced or not synced), false otherwise.
            */
            bool IsReady();

            /*!
            * @brief Gets the current time.
            * @param hours Pointer to store the current hours.
            * @param minutes Pointer to store the current minutes.
            * @param seconds Pointer to store the current seconds.
            * @return True if successful, false otherwise.
            */
            bool GetCurrentTime(std::string* hours, std::string* minutes, std::string* seconds);

            /*!
            * @brief Sets the time zone for synchronization.
            * @param timeZone Time zone to set.
            * @return True if successful, false otherwise.
            */
            bool SetTimeZone(std::string timeZone);

            /*!
            * @brief Saves a string to EEPROM at a specified position.
            * @param position EEPROM position to save the string.
            * @param str String to save.
            */
            void SaveStringToEeprom(const int position, std::string str);

            /*!
            * @brief Reads a string from EEPROM at a specified position.
            * @param position EEPROM position to read the string.
            * @return Read string.
            */
            std::string ReadStringFromEeprom(const int position);

        private:

            enum class RTC_STATE 
            {
                INIT,
                START_SYNC,
                WAITING_RESPONSE,
                SYNCED,
                NOT_SYNCED
            };

            /*!
            * @brief Constructor for the RealTimeClock class.
            * @param sdaPin SDA pin for I2C communication.
            * @param sclPin SCL pin for I2C communication.
            * @param address I2C address of the RTC.
            * @param eepromAddr EEPROM address for RTC configuration.
            */
            RealTimeClock(PinName sdaPin, PinName sclPin, uint8_t address, uint8_t eepromAddr);

            ~RealTimeClock() = default;
            RealTimeClock(const RealTimeClock&) = delete;
            RealTimeClock& operator=(const RealTimeClock&) = delete;

            /*!
            * @brief Synchronizes the RTC time from a response.
            * @param response HTTP response with time information.
            * @return True if synchronization successful, false otherwise.
            */
            bool _SyncFromResponse(std::string response);

            /*!
            * @brief Sets the RTC time.
            * @param time tm structure containing time information.
            */
            void _Set(const tm time);

            /*!
            * @brief Converts Binary Coded Decimal (BCD) to Decimal.
            * @param num BCD number to convert.
            * @return Decimal equivalent.
            */
            uint8_t _Bcd2Dec(uint8_t num);

            /*!
            * @brief Converts Decimal to Binary Coded Decimal (BCD).
            * @param num Decimal number to convert.
            * @return BCD equivalent.
            */
            uint8_t _Dec2Bcd(uint8_t num);

            static RealTimeClock* mInstance;
            I2C mRtcCom;
            RTC_STATE mState;
            const uint8_t mAddress;
            Memory mMemory;
            Util::Delay mRtcDelay;
            std::map<std::string, std::string> mTimeZonesMap;
            std::string mCurrentTimeZone;
            int mNumSyncAtempts;
    };
} // namespace Subsystems

#endif // REAL_TIME_CLOCK_H