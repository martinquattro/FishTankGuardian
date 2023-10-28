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
static Util::nonBlockingDelay_t smartHomeSystemDelay;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void FishTankGuardian::Init()
{
    Util::TickInit();
    Util::NonBlockingDelayInit(&smartHomeSystemDelay, SYSTEM_TIME_INCREMENT_MS);

    if (mInstance == nullptr)
    {
        mInstance = new FishTankGuardian();
    }
}

//----static-------------------------------------------------------------------
FishTankGuardian* FishTankGuardian::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void FishTankGuardian::Update()
{
    if(Util::NonBlockingDelayRead(&smartHomeSystemDelay)) 
    {
    }
}

//=====[Implementations of private functions]==================================

} // namespace Subsystems
