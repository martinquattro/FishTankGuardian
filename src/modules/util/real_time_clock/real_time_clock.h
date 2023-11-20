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

#include "mbed.h"

namespace Util { 

    class RealTimeClock 
    {
        public:

            //! Initialize the rtc
            static void Init();

            //! Returns the rtc object
            static RealTimeClock* GetInstance();

            //!
            tm GetCurrentTime();

            //!
            void Sync();


        private:

            RealTimeClock(PinName sdaPin, PinName sclPin, uint8_t adress);
            ~RealTimeClock() = default;
            RealTimeClock(const RealTimeClock&) = delete;
            RealTimeClock& operator=(const RealTimeClock&) = delete;


            //!
            void _Set(const tm time);

            //! Convert Binary Coded Decimal (BCD) to Decimal
            uint8_t _Bcd2Dec(uint8_t num);

            //!Convert Decimal to Binary Coded Decimal (BCD)
            uint8_t _Dec2Bcd(uint8_t num);

            static RealTimeClock* mInstance;
            I2C mRtcCom;
            const uint8_t mAddress;


    };

} // namespace Util

#endif // REAL_TIME_CLOCK_H