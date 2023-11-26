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
#include "wifi_com.h"

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
    Subsystems::RealTimeClock::Init();
    Subsystems::FoodFeeder::Init();
    Subsystems::WaterMonitor::Init();
    Subsystems::TelegramBot::Init();
    Subsystems::UserInterface::Init();
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
        int initTickCounter = static_cast<int>(Util::Tick::GetTickCounter());
        Subsystems::FoodFeeder::GetInstance()->Update();
        int endTickCounter = static_cast<int>(Util::Tick::GetTickCounter());
        DEBUG_PRINT("FishTankGuardian - Time in FoodFeeder Update = [%d] ms\r\n", (endTickCounter - initTickCounter));

        initTickCounter = endTickCounter;
        Subsystems::WaterMonitor::GetInstance()->Update();
        endTickCounter = static_cast<int>(Util::Tick::GetTickCounter());
        DEBUG_PRINT("FishTankGuardian - Time in WaterMonitor Update = [%d] ms\r\n", (endTickCounter - initTickCounter));

        initTickCounter = endTickCounter;
        Subsystems::UserInterface::GetInstance()->Update();
        endTickCounter = static_cast<int>(Util::Tick::GetTickCounter());
        DEBUG_PRINT("FishTankGuardian - Time in UserInterface Update = [%d] ms\r\n", (endTickCounter - initTickCounter));

    }
    Drivers::WiFiCom::GetInstance()->Update();
    Subsystems::RealTimeClock::GetInstance()->Update();
    Subsystems::TelegramBot::GetInstance()->Update();
}

//=====[Implementations of private functions]==================================

//-----------------------------------------------------------------------------
FishTankGuardian::FishTankGuardian() 
    : mDelay(SYSTEM_TIME_INCREMENT_MS)
{}

} // namespace Subsystems
