/*!****************************************************************************
 * @file fish_tank_guardian.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "fish_tank_guardian.h"

#include "arm_book_lib.h"
#include "delay.h"
#include "food_feeder.h"
#include "mbed.h"
#include "pc_serial_com.h"
#include "real_time_clock.h"
#include "telegram_bot.h"
#include "user_interface.h"
#include "water_monitor.h"

namespace Subsystems {

//=====[Declaration and initialization of private global variables]============

FishTankGuardian* FishTankGuardian::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void FishTankGuardian::Init()
{
    if (mInstance == nullptr)
    {
        mInstance = new FishTankGuardian();
    }
  
    Util::PcSerialCom::Init();
    Util::Tick::Init();
    Util::RealTimeClock::Init();
    Subsystems::FoodFeeder::Init();
    Subsystems::WaterMonitor::Init();
    Subsystems::TelegramBot::Init();
    Subsystems::UserInterface::Init();


    // debug ------------------------------------------------------------
    Util::RealTimeClock::GetInstance()->Sync();

    std:string feedTime = "21:00:15";
    Subsystems::FoodFeeder::GetInstance()->AddFeedTime(feedTime);

    std::vector<std::string> feedTimes = Subsystems::FoodFeeder::GetInstance()->GetFeedTimes();
    DEBUG_PRINT("GetFeedTimes = %s\r\n", feedTimes[0].c_str());
}

//----static-------------------------------------------------------------------
FishTankGuardian* FishTankGuardian::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void FishTankGuardian::Update()
{
    if(mDelay.HasFinished()) 
    {
        Subsystems::FoodFeeder::GetInstance()->Update();
        Subsystems::WaterMonitor::GetInstance()->Update();
        Subsystems::UserInterface::GetInstance()->Update();

        // debug ---------------------------------
        // DEBUG_PRINT("tickCounter = %d\r\n", static_cast<int>(Util::Tick::GetTickCounter()));   

        DEBUG_PRINT("%s\r\n", Util::RealTimeClock::GetInstance()->GetCurrentTime().c_str());
    }
    Subsystems::TelegramBot::GetInstance()->Update();
}

//=====[Implementations of private functions]==================================

//-----------------------------------------------------------------------------
FishTankGuardian::FishTankGuardian() 
    : mDelay(SYSTEM_TIME_INCREMENT_MS)
{}

} // namespace Subsystems
