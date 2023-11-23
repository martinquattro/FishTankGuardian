/*!****************************************************************************
 * @file food_feeder.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "food_feeder.h"

#include <string>
#include "arm_book_lib.h"
#include "motor.h"
#include "real_time_clock.h"

namespace Subsystems {

//=====[Declaration and initialization of private global variables]============

FoodFeeder* FoodFeeder::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void FoodFeeder::Init()
{
    DEBUG_PRINT("FoodFeeder::Init() - Initiating...\r\n");

    if (mInstance == nullptr)
    {
        mInstance = new FoodFeeder();
    }

    Drivers::Motor::Init();

    DEBUG_PRINT("FoodFeeder::Init() - Initiating Finished.\r\n");
}

//----static-------------------------------------------------------------------
FoodFeeder* FoodFeeder::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void FoodFeeder::Update()
{
    std::string currentTime = Util::RealTimeClock::GetInstance()->GetCurrentTime();
    if (_IsTimeToFeed(currentTime))
    {
        Drivers::Motor::GetInstance()->Rotate();
    }
}

//-----------------------------------------------------------------------------
bool FoodFeeder::AddFeedTime(std::string newFeedTime)
{
    // Check if the length is correct
    DEBUG_PRINT("newFeedTime = %s\r\n", newFeedTime.c_str());
    if (newFeedTime.size() != 8 || !_IsValidTimeFormat(newFeedTime.c_str())) 
    {
        return false;
    }
    
    Util::RealTimeClock::GetInstance()->SaveStringToEeprom(newFeedTime);

    return true;
}

//-----------------------------------------------------------------------------
std::vector<std::string> FoodFeeder::GetFeedTimes()
{
    std::vector<std::string> feedTimes;

    // Read the feed time from EEPROM
    std::string feedTime = Util::RealTimeClock::GetInstance()->ReadStringFromEeprom();
    feedTimes.push_back(feedTime);

    return feedTimes;}

//=====[Implementations of private functions]==================================

//-----------------------------------------------------------------------------
FoodFeeder::FoodFeeder()
{

}

//-----------------------------------------------------------------------------
bool FoodFeeder::_IsValidTimeFormat(const char* time) 
{
    if (   std::isdigit(time[0]) && std::isdigit(time[1]) 
        && time[2] == ':' 
        && std::isdigit(time[3]) && std::isdigit(time[4])
        && time[5] == ':' 
        && std::isdigit(time[6]) && std::isdigit(time[7])) 
    {
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
bool FoodFeeder::_IsTimeToFeed(std::string currentTime)
{

    std::vector<std::string> feedTimes = Subsystems::FoodFeeder::GetInstance()->GetFeedTimes();

    for (const auto& feedTimeStr : feedTimes)
    {
        if (!strcmp(feedTimeStr.c_str(), currentTime.c_str()))
        {
            return true;
        }
    }

    return false;
}

} // namespace Subsystems
