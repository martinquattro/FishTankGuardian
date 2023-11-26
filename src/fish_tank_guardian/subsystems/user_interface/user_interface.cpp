/*!****************************************************************************
 * @file user_interface.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "user_interface.h"

#include "arm_book_lib.h"
#include "display.h"
#include "real_time_clock.h"
#include "water_monitor.h"
#include "food_feeder.h"

namespace Subsystems {

//=====[Declaration and initialization of private global variables]============

UserInterface* UserInterface::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void UserInterface::Init()
{
    DEBUG_PRINT("UserInterface::Init() - Initiating...\r\n");

    if (mInstance == nullptr)
    {
        mInstance = new UserInterface();
    }

    Drivers::Display::Init(DISPLAY_TYPE_LCD_HD44780, DISPLAY_CONNECTION_GPIO_4BITS);
    Drivers::Display::Clear();

    Drivers::Display::WriteCharPosition(0,0);
    Drivers::Display::Write(" FISH TANK GUARDIAN ");

    Drivers::Display::WriteCharPosition(0,1);
    Drivers::Display::Write("Next Feed: ");

    Drivers::Display::WriteCharPosition(0,2);
    Drivers::Display::Write("Temp:     'C________");
    
    Drivers::Display::WriteCharPosition(0,3);
    Drivers::Display::Write("TDS:    ppm|        ");

    DEBUG_PRINT("UserInterface::Init() - Initiating Finished.\r\n");
}

//----static-------------------------------------------------------------------
UserInterface* UserInterface::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void UserInterface::Update()
{
    char buffer[60];

    // Next Feed Time
    {
        std::string nextFeedTime = Subsystems::FoodFeeder::GetInstance()->GetNextFeedTime();

        Drivers::Display::WriteCharPosition(FEED_FEED_POSITION_X, FEED_FEED_POSITION_Y);
        Drivers::Display::Write(nextFeedTime.c_str());
    }

    // Temperature
    {
        const float tempReading = Subsystems::WaterMonitor::GetInstance()->GetTempReading();
        std::sprintf(buffer, "%.1f", tempReading);
        Drivers::Display::WriteCharPosition(TEMP_POSITION_X, TEMP_POSITION_Y);
        Drivers::Display::Write(buffer);
    }

    // Tds
    {
        const int tdsReading = Subsystems::WaterMonitor::GetInstance()->GetTdsReading();
        std::sprintf(buffer, "%03d", tdsReading);
        Drivers::Display::WriteCharPosition(TDS_POSITION_X, TDS_POSITION_Y);
        Drivers::Display::Write(buffer);
    }

    // Current Time
    {    
        std::string hours, minutes, seconds, currentTime;
        Util::RealTimeClock::GetInstance()->GetCurrentTime(&hours, &minutes, &seconds);

        currentTime += hours;
        currentTime += ":";
        currentTime += minutes;
        currentTime += ":";
        currentTime += seconds;

        Drivers::Display::WriteCharPosition(TIME_POSITION_X, TIME_POSITION_Y);
        Drivers::Display::Write(currentTime.c_str());
    }
}

//=====[Implementations of private functions]==================================

} // namespace Subsystems
