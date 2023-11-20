/*!****************************************************************************
 * @file fish_tank_guardian.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "fish_tank_guardian.h"

#include "delay.h"
#include "food_feeder.h"
#include "mbed.h"
#include "pc_serial_com.h"
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
        Subsystems::FoodFeeder::GetInstance()->Update();
        Subsystems::WaterMonitor::GetInstance()->Update();
        Subsystems::UserInterface::GetInstance()->Update();
    }
    Subsystems::TelegramBot::GetInstance()->Update();
}

//=====[Implementations of private functions]==================================

//-----------------------------------------------------------------------------
FishTankGuardian::FishTankGuardian() 
    : mDelay(SYSTEM_TIME_INCREMENT_MS)
{}

} // namespace Subsystems
