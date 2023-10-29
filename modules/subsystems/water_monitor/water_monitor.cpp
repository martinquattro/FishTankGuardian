/*!****************************************************************************
 * @file water_monitor.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "water_monitor.h"

namespace Subsystems {

//=====[Declaration and initialization of private global variables]============

WaterMonitor* WaterMonitor::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void WaterMonitor::Init()
{
    if (mInstance == nullptr)
    {
        mInstance = new WaterMonitor();
    }
}

//----static-------------------------------------------------------------------
WaterMonitor* WaterMonitor::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void WaterMonitor::Update()
{
    // TODO: Update Drivers controlled by the WaterMonitor Subsystem
}

//=====[Implementations of private functions]==================================

} // namespace Subsystems
