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
#include "wifi_com.h"
#include "Json.h"
#include "commands.h"

namespace Subsystems {

//=====[Declaration and initialization of private global variables]============

RealTimeClock* RealTimeClock::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void RealTimeClock::Init()
{
    DEBUG_PRINT("RealTimeClock - Initiating...\r\n");

    if (mInstance == nullptr)
    {
        mInstance = new RealTimeClock(RTC_PIN_SDA, RTC_PIN_SCL, RTC_ADDRESS_ID, RTC_EEPROM_ADDRESS);
    }

    DEBUG_PRINT("RealTimeClock - [OK] Initialized\r\n");
}

//----static-------------------------------------------------------------------
RealTimeClock* RealTimeClock::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void RealTimeClock::Update()
{
    switch(mState)
    {
        case RTC_STATE::START_SYNC:
        {
            if ((Drivers::WiFiCom::GetInstance()->IsBusy()) == false)
            {
                Drivers::WiFiCom::GetInstance()->Request(RTC_GET_TIME_URL + mCurrentTimeZone);
                mState = RTC_STATE::WAITING_RESPONSE;
                mRtcDelay.Start(DELAY_10_SECONDS);
            }
        }
        break;
        case RTC_STATE::WAITING_RESPONSE:
        {
            std::string response;
            if (mRtcDelay.HasFinished())
            {
                mState = RTC_STATE::NOT_SYNCED;
                DEBUG_PRINT("RealTimeClock - [ERROR] Not able to synced\r\n");
            }
            else if (Drivers::WiFiCom::GetInstance()->GetGetResponse(&response))
            {
                if (_SyncFromResponse(response))
                {
                    mState = RTC_STATE::SYNCED;
                    DEBUG_PRINT("RealTimeClock - [OK] Synced correctly\r\n");
                }
                else
                {
                    mState = RTC_STATE::NOT_SYNCED;
                    DEBUG_PRINT("RealTimeClock - [ERROR] Something went wrong. Not synqued\r\n");
                }
            }
        }
        break;
        case RTC_STATE::SYNCED:
        break;
        case RTC_STATE::NOT_SYNCED:
        break;
    }
}

//-----------------------------------------------------------------------------
bool RealTimeClock::SetTimeZone(std::string timeZone)
{
    if (mTimeZonesMap.find(timeZone) != mTimeZonesMap.end())
    {
        mState = RTC_STATE::START_SYNC;
        mCurrentTimeZone = mTimeZonesMap[timeZone];
        DEBUG_PRINT("RealTimeClock - mCurrentTimeZone = [%s]\r\n", mCurrentTimeZone.c_str());

        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
bool RealTimeClock::GetCurrentTime(std::string* hours, std::string* minutes, std::string* seconds)
{
    uint8_t currentSeconds, currentMinutes, currentHours, currentDay, currentMonth, currentYear;

    if (mState == RTC_STATE::SYNCED)
    {
        mRtcCom.start();
        mRtcCom.write(mAddress | 0);
        mRtcCom.write(0x00);

        mRtcCom.start();

        mRtcCom.write(mAddress | 1);
        currentSeconds  = _Bcd2Dec(mRtcCom.read(1));
        currentMinutes  = _Bcd2Dec(mRtcCom.read(1));
        currentHours    = _Bcd2Dec(mRtcCom.read(1));

        mRtcCom.read(1);
        currentDay      = _Bcd2Dec(mRtcCom.read(1));
        currentMonth    = _Bcd2Dec(mRtcCom.read(1));
        currentYear     = _Bcd2Dec(mRtcCom.read(0));

        mRtcCom.stop();

        char buffer[60];

        std::sprintf(buffer, "%02d", currentHours);
        (*hours) = buffer;

        std::sprintf(buffer, "%02d", currentMinutes);
        (*minutes) = buffer;

        std::sprintf(buffer, "%02d", currentSeconds);
        (*seconds) = buffer;

        return true;
    }
    else
    {
        return false;
    }
}

//-----------------------------------------------------------------------------
void RealTimeClock::SaveStringToEeprom(const int position, std::string str) 
{
    mMemory.write(position, str);
}

//-----------------------------------------------------------------------------
std::string RealTimeClock::ReadStringFromEeprom(const int position) 
{
    std::string str;
    mMemory.read(position, str);

    return str;
}

//=====[Implementations of private functions]==================================

RealTimeClock::RealTimeClock(PinName sdaPin, PinName sclPin, uint8_t address, uint8_t eepromAddr)
    : mRtcCom(sdaPin, sclPin)
    , mAddress(address << 1)
    , mMemory(sdaPin, sclPin, eepromAddr)
    , mRtcDelay(0)
{
    mState = RTC_STATE::START_SYNC;
    mCurrentTimeZone = "America/Buenos_Aires";         // debug

    mRtcCom.start();
    mRtcCom.write(mAddress | 0);
    mRtcCom.write(0x07u);
    mRtcCom.write(0x00u);
    mRtcCom.stop();

    // only time zones in America for now
    mTimeZonesMap["-8"] = "America/Los_Angeles";
    mTimeZonesMap["-7"] = "America/Denver";
    mTimeZonesMap["-6"] = "America/Chicago";
    mTimeZonesMap["-5"] = "America/Bogota";
    mTimeZonesMap["-4"] = "America/Caracas";
    mTimeZonesMap["-3"] = "America/Buenos_Aires";
    mTimeZonesMap["-2"] = "America/Noronha";
    mTimeZonesMap["-1"] = "America/Scoresbysund";
}

//-----------------------------------------------------------------------------
bool RealTimeClock::_SyncFromResponse(std::string response)
{
    Json json(response.c_str(), response.length(), 100);

        DEBUG_PRINT("RealTimeClock - response = [%s]\r\n", response.c_str());

    if (json.isValidJson() && (response.compare("\"Invalid Timezone\"") != 0) && (response.compare(RESULT_ERROR) != 0))
    {
        int seconds, minutes, hours, day, month, year;

        json.tokenIntegerValue(json.findChildIndexOf(json.findKeyIndex("seconds")),     seconds);
        json.tokenIntegerValue(json.findChildIndexOf(json.findKeyIndex("minute")),      minutes);
        json.tokenIntegerValue(json.findChildIndexOf(json.findKeyIndex("hour")),        hours);
        json.tokenIntegerValue(json.findChildIndexOf(json.findKeyIndex("day")),         day);
        json.tokenIntegerValue(json.findChildIndexOf(json.findKeyIndex("month")),       month);
        json.tokenIntegerValue(json.findChildIndexOf(json.findKeyIndex("year")),        year);

        struct tm rtcTime;

        rtcTime.tm_year = year;
        rtcTime.tm_mon  = month;
        rtcTime.tm_mday = day;
        rtcTime.tm_hour = hours;
        rtcTime.tm_min  = minutes;
        rtcTime.tm_sec  = seconds;
        rtcTime.tm_isdst = -1;

        _Set(rtcTime);

        char dateStr[80];
        strftime(dateStr, sizeof(dateStr), "%H:%M:%S", &rtcTime);
        DEBUG_PRINT("RealTimeClock - [OK] Time synqued to %s\r\n", dateStr);

        return true;
    }
    {
        return false;
    }
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

} // namespace Subsystems

