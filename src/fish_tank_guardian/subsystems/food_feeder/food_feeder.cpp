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
    tm currentTime = Util::RealTimeClock::GetInstance()->GetCurrentTime();
    if (_IsTimeToFeed(currentTime))
    {
        Drivers::Motor::GetInstance()->Rotate();
    }
}

//=====[Implementations of private functions]==================================

//-----------------------------------------------------------------------------
bool FoodFeeder::_IsTimeToFeed(const tm currentTime)
{
    std::string feedTimeStr = "20:45:0";

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", &currentTime);
    std::string currentIimeStr(buffer);

    return (!strcmp(feedTimeStr.c_str(), currentIimeStr.c_str()));
}

} // namespace Subsystems
