/*!****************************************************************************
 * @file real_time_clock.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef REAL_TIME_CLOCK_H
#define REAL_TIME_CLOCK_H

#define RTC_PIN_SDA        PB_11
#define RTC_PIN_SCL        PB_10
#define RTC_ADDRESS_ID     0x68
#define RTC_EEPROM_ADDRESS 0x50
#define RTC_GET_TIME_URL "https://www.timeapi.io/api/Time/current/zone?timeZone="

#include "mbed.h"
#include "memory.h"
#include <string>
#include "delay.h"
#include <map>

namespace Subsystems { 

    class RealTimeClock 
    {
        public:

            //! Initialize the rtc
            static void Init();

            //! Returns the rtc object
            static RealTimeClock* GetInstance();

            //!
            bool GetCurrentTime(std::string* hours, std::string* minutes, std::string* seconds);

            //!
            void Update();

            //!
            bool SetTimeZone(std::string timeZone);

            //!
            void SaveStringToEeprom(const int position, std::string str);

            //!
            std::string ReadStringFromEeprom(const int position);

        private:

            enum class RTC_STATE
            {
                START_SYNC,
                WAITING_RESPONSE,
                SYNCED,
                NOT_SYNCED
            };

            RealTimeClock(PinName sdaPin, PinName sclPin, uint8_t address, uint8_t eepromAddr);
            ~RealTimeClock() = default;
            RealTimeClock(const RealTimeClock&) = delete;
            RealTimeClock& operator=(const RealTimeClock&) = delete;


            //!
            bool _SyncFromResponse(std::string response);

            //!
            void _Set(const tm time);

            //! Convert Binary Coded Decimal (BCD) to Decimal
            uint8_t _Bcd2Dec(uint8_t num);

            //!Convert Decimal to Binary Coded Decimal (BCD)
            uint8_t _Dec2Bcd(uint8_t num);

            static RealTimeClock* mInstance;

            I2C             mRtcCom;
            RTC_STATE       mState;
            const uint8_t   mAddress;
            Memory          mMemory;
            Util::Delay     mRtcDelay;
            std::map<std::string, std::string> mTimeZonesMap;
            std::string     mCurrentTimeZone;
};

} // namespace Subsystems

#endif // REAL_TIME_CLOCK_H