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
    DEBUG_PRINT("FoodFeeder - Initiating...\r\n");

    if (mInstance == nullptr)
    {
        mInstance = new FoodFeeder();
    }

    Drivers::Motor::Init();

    DEBUG_PRINT("FoodFeeder - [OK] Initialized\r\n");
}

//----static-------------------------------------------------------------------
FoodFeeder* FoodFeeder::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void FoodFeeder::Update()
{
    std::string currentHours, currentMinutes, currentSeconds;
    const bool success = Subsystems::RealTimeClock::GetInstance()->GetCurrentTime(&currentHours, &currentMinutes, &currentSeconds);
    
    if (success)
    {
        std::string currentTime = currentHours + ":" + currentMinutes + ":" + currentSeconds;

        int numFeeds;
        if (_IsTimeToFeed(currentTime, &numFeeds))
        {
            Feed(numFeeds);
        }
    }
}

//-----------------------------------------------------------------------------
bool FoodFeeder::Feed(const int numFeeds)
{
    if((numFeeds > 1) || (numFeeds <= MAX_NUM_FEEDS)) 
    {
        DEBUG_PRINT("FoodFeeder - Initiating feeding with [%d] units of food...\r\n", numFeeds);
        Drivers::Motor::GetInstance()->Rotate(numFeeds);

        return true;
    }
    
    DEBUG_PRINT("FoodFeeder - [ERROR] Invalid [%d] units of food\r\n", numFeeds);
    return false;
}

//-----------------------------------------------------------------------------
bool FoodFeeder::EraseFeedTime(const int feedTimeSlot)
{
    if ((feedTimeSlot < 0) || (feedTimeSlot >= FEED_TIMES_SLOTS_NUM))
    {
        DEBUG_PRINT("FoodFeeder - [ERROR] Invalid feed time slot to erase = [%d]\r\n", feedTimeSlot);
        return false;
    }

    std::string nullFeedTime = "----------";

    // Calculate the EEPROM position based on the slot size
    int eepromPosition = FEED_TIMES_EEPROM_START + (feedTimeSlot * FEED_TIME_BYTES);

    // Save the null feed time to EEPROM
    Subsystems::RealTimeClock::GetInstance()->SaveStringToEeprom(eepromPosition, nullFeedTime);

    return true;
}

//-----------------------------------------------------------------------------
bool FoodFeeder::AddFeedTime(std::string newFeedTime, const int numFeeds, const int feedTimeSlot)
{
    // Valdiate if it is a valid feedtime, slot and num of feeds
    if (newFeedTime.size() != 8 
        || !(_IsValidTimeFormat(newFeedTime.c_str()) )
        || (feedTimeSlot < 0) 
        || (feedTimeSlot >= FEED_TIMES_SLOTS_NUM)
        || (numFeeds < 1)
        || (numFeeds > MAX_NUM_FEEDS)) 
    {
        DEBUG_PRINT("FoodFeeder - [ERROR] Invalid feed time [%s] or num feeds = [%d] or slot = [%d]\r\n", newFeedTime.c_str(), numFeeds, feedTimeSlot);
        return false;
    }
    
    // Create the HH:MM:SS-X format to save in eeprom
    std::string feedTimeStr = newFeedTime + '-' + std::to_string(numFeeds);

    // Calculate the EEPROM position based on the slot size
    int eepromPosition = FEED_TIMES_EEPROM_START + (feedTimeSlot * FEED_TIME_BYTES);

    // Save the feed time to EEPROM
    Subsystems::RealTimeClock::GetInstance()->SaveStringToEeprom(eepromPosition, feedTimeStr);

    DEBUG_PRINT("FoodFeeder - [OK] New feed time added [%s]\r\n", newFeedTime.c_str());

    return true;
}

//-----------------------------------------------------------------------------
std::vector<FoodFeeder::FeedTimeInfo> FoodFeeder::GetFeedTimes()
{
    std::vector<FeedTimeInfo> feedTimes;
    
    for (int i = 0; i < FEED_TIMES_SLOTS_NUM; ++i)
    {
        const int slotEepromStartPosition = FEED_TIMES_EEPROM_START + (i * FEED_TIME_BYTES);
        std::string feedTime = Subsystems::RealTimeClock::GetInstance()->ReadStringFromEeprom(slotEepromStartPosition);

        if (_IsValidTimeFormat(feedTime.c_str()))
        {
            size_t dashPos = feedTime.find('-');
            std::string timePart = feedTime.substr(0, dashPos);
            std::string feedsPart = feedTime.substr(dashPos + 1);

            FeedTimeInfo feedTimeInfo;
            feedTimeInfo.mSlot = i;
            feedTimeInfo.mNumFeeds = stoi(feedsPart);
            feedTimeInfo.mFeedTime = timePart;

            feedTimes.push_back(feedTimeInfo);
        }
    }

    return feedTimes;
}

//-----------------------------------------------------------------------------
std::string FoodFeeder::GetNextFeedTime()
{
    std::string nextFeedTime = "";
    std::string currentHours, currentMinutes, currentSeconds;

    const bool success = Subsystems::RealTimeClock::GetInstance()->GetCurrentTime(&currentHours, &currentMinutes, &currentSeconds);

    if (success)
    {
        std::string currentTime = currentHours + ":" + currentMinutes + ":" + currentSeconds;

        std::vector<FeedTimeInfo> feedTimes = GetFeedTimes();

        for (const auto& feedTimeInfo : feedTimes)
        {
            if (feedTimeInfo.mFeedTime > currentTime && (nextFeedTime == "" || feedTimeInfo.mFeedTime < nextFeedTime))
            {
                nextFeedTime = feedTimeInfo.mFeedTime;
            }
        }

        if (nextFeedTime == "")
        {
            if (feedTimes.size())
            {
                // No more feeds today
                nextFeedTime = "Tomorrow ";
            }
            else
            {
                nextFeedTime = "None set ";
            }
        }

    }

    return nextFeedTime;
}

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
bool FoodFeeder::_IsTimeToFeed(std::string currentTime, int* numFeeds)
{
    std::vector<FeedTimeInfo> feedTimes = GetFeedTimes();

    for (const auto& feedTimeInfo : feedTimes)
    {
        if (feedTimeInfo.mFeedTime == currentTime)
        {
            (*numFeeds) = feedTimeInfo.mNumFeeds;
            return true;
        }
    }

    return false;
}

} // namespace Subsystems
