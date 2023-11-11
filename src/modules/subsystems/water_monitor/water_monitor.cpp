/*!****************************************************************************
 * @file water_monitor.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "water_monitor.h"
#include "tds_sensor.h"
#include "temperature_sensor.h"

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

    Drivers::TemperatureSensor::Init();
    Drivers::TdsSensor::Init();
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
