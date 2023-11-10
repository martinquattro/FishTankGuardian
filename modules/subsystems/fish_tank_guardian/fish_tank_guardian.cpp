/*!****************************************************************************
 * @file fish_tank_guardian.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "fish_tank_guardian.h"
#include "water_monitor.h"
#include "food_feeder.h"
#include "user_interface.h"
#include "delay.h"
#include "pc_serial_com.h"

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

    Drivers::PcSerialCom::Init();
    Util::Tick::Init();
    Subsystems::FoodFeeder::Init();
    Subsystems::WaterMonitor::Init();
    
    Util::Delay::Init(SYSTEM_TIME_INCREMENT_MS);
}

//----static-------------------------------------------------------------------
FishTankGuardian* FishTankGuardian::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void FishTankGuardian::Update()
{
    if(Util::Delay::GetInstance()->HasFinished()) 
    {
        Subsystems::FoodFeeder::GetInstance()->Update();
        Subsystems::WaterMonitor::GetInstance()->Update();
        Subsystems::UserInterface::GetInstance()->Update();
    }
}

//=====[Implementations of private functions]==================================

} // namespace Subsystems
