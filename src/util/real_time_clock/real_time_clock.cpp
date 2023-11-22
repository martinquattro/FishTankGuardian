/*!****************************************************************************
 * @file real_time_clock.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "real_time_clock.h"

#include <cstdarg>
#include "arm_book_lib.h"

namespace Util {

//=====[Declaration and initialization of private global variables]============

RealTimeClock* RealTimeClock::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void RealTimeClock::Init()
{
    DEBUG_PRINT("RealTimeClock::Init() - Initiating...\r\n");

    if (mInstance == nullptr)
    {
        mInstance = new RealTimeClock(RTC_PIN_SDA, RTC_PIN_SCL, RTC_ADDRESS_ID);
    }

    mInstance->Sync();              // debug

    DEBUG_PRINT("RealTimeClock::Init() - Initiating Finished.\r\n");
}

//----static-------------------------------------------------------------------
RealTimeClock* RealTimeClock::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
tm RealTimeClock::GetCurrentTime()
{
    uint8_t seconds, minutes, hours, day, month, year;

    mRtcCom.start();
    mRtcCom.write(mAddress | 0);
    mRtcCom.write(0x00);

    mRtcCom.start();

    mRtcCom.write(mAddress | 1);
    seconds  = _Bcd2Dec(mRtcCom.read(1));
    minutes  = _Bcd2Dec(mRtcCom.read(1));
    hours    = _Bcd2Dec(mRtcCom.read(1));

    mRtcCom.read(1);
    day      = _Bcd2Dec(mRtcCom.read(1));
    month    = _Bcd2Dec(mRtcCom.read(1));
    year     = _Bcd2Dec(mRtcCom.read(0));

    mRtcCom.stop();

    tm currentTime;
    currentTime.tm_sec  = seconds;
    currentTime.tm_min  = minutes;
    currentTime.tm_hour = hours;
    currentTime.tm_mday = day;
    currentTime.tm_mon  = month;
    currentTime.tm_year = year;

    return currentTime;
}

//-----------------------------------------------------------------------------
void RealTimeClock::Sync()
{
    DEBUG_PRINT("RealTimeClock::Sync() - Syncing date and time...\r\n");

    struct tm rtcTime;

    rtcTime.tm_year = 123;
    rtcTime.tm_mon  = 11;
    rtcTime.tm_mday = 23;
    rtcTime.tm_hour = 20;
    rtcTime.tm_min  = 44;
    rtcTime.tm_sec  = 45;
    rtcTime.tm_isdst = -1;

    mktime(&rtcTime);

    _Set(rtcTime);

    char dateStr[80];
    strftime(dateStr, sizeof(dateStr), "%H:%M:%S", &rtcTime);
    DEBUG_PRINT("RealTimeClock::Sync() - Time synqued to %s\r\n", dateStr);
}

//=====[Implementations of private functions]==================================

RealTimeClock::RealTimeClock(PinName sdaPin, PinName sclPin, uint8_t address)
    : mRtcCom(sdaPin, sclPin)
    , mAddress(address << 1)
{
    mRtcCom.start();
    mRtcCom.write(mAddress | 0);
    mRtcCom.write(0x07u);
    mRtcCom.write(0x00u);
    mRtcCom.stop();
}

//-----------------------------------------------------------------------------
void RealTimeClock::_Set(const tm time)
{
    mRtcCom.start();
    mRtcCom.write(mAddress | 0);
    mRtcCom.write(0x00u);
    mRtcCom.write(_Dec2Bcd(time.tm_sec));
    mRtcCom.write(_Dec2Bcd(time.tm_min));
    mRtcCom.write(_Dec2Bcd(time.tm_hour));

    mRtcCom.write(1 & 0x07);
    mRtcCom.write(_Dec2Bcd(time.tm_mday));
    mRtcCom.write(_Dec2Bcd(time.tm_mon));
    mRtcCom.write(_Dec2Bcd(time.tm_year));
    mRtcCom.stop();
}

//-----------------------------------------------------------------------------
uint8_t RealTimeClock::_Bcd2Dec(uint8_t num)
{
  return ((num / 16 * 10) + (num % 16));
}

//-----------------------------------------------------------------------------
uint8_t RealTimeClock::_Dec2Bcd(uint8_t num)
{
  return ((num / 10 * 16) + (num % 10));
}

} // namespace Util

