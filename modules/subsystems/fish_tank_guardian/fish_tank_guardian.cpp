/*!****************************************************************************
 * @file fish_tank_guardian.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "fish_tank_guardian.h"
#include "non_blocking_delay.h"

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

    Util::Tick::Init();
    Util::NonBlockingDelay::Init(SYSTEM_TIME_INCREMENT_MS);
}

//----static-------------------------------------------------------------------
FishTankGuardian* FishTankGuardian::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void FishTankGuardian::Update()
{
    if(!Util::NonBlockingDelay::GetInstance()->HasFinished()) 
    {
    }
}

//=====[Implementations of private functions]==================================

} // namespace Subsystems
