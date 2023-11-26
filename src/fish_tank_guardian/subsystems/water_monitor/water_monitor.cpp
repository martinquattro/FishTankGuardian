/*!****************************************************************************
 * @file water_monitor.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "water_monitor.h"

#include "arm_book_lib.h"
#include "temperature_sensor.h"
#include "tds_sensor.h"

namespace Subsystems {

//=====[Declaration and initialization of private global variables]============

WaterMonitor* WaterMonitor::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void WaterMonitor::Init()
{
    DEBUG_PRINT("WaterMonitor::Init() - Initiating...\r\n");

    if (mInstance == nullptr)
    {
        mInstance = new WaterMonitor();
    }

    Drivers::TemperatureSensor::Init();
    Drivers::TdsSensor::Init();

    DEBUG_PRINT("WaterMonitor::Init() - Initiating Finished.\r\n");
}

//----static-------------------------------------------------------------------
WaterMonitor* WaterMonitor::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void WaterMonitor::Update()
{
    Drivers::TemperatureSensor::GetInstance()->Update();
    Drivers::TdsSensor::GetInstance()->Update(GetTempReading());            // tds sensor uses temperature reading to have a better reading
}

//-----------------------------------------------------------------------------
int WaterMonitor::GetTdsReading()
{
    return (Drivers::TdsSensor::GetInstance()->GetLastReading());
}

//-----------------------------------------------------------------------------
float WaterMonitor::GetTempReading()
{
    return (Drivers::TemperatureSensor::GetInstance()->GetLastReading());
}

//=====[Implementations of private functions]==================================

} // namespace Subsystems
