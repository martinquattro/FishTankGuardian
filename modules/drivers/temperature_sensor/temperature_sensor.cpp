/*!****************************************************************************
 * @file temperature_sensor.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "temperature_sensor.h"

namespace Drivers {

//=====[Declaration and initialization of private global variables]============

TemperatureSensor* TemperatureSensor::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------------
void TemperatureSensor::Init(int pin)
{
    if (mInstance == nullptr)
    {
        mInstance = new TemperatureSensor(pin);
    }
}

//----static-------------------------------------------------------------------------
TemperatureSensor* TemperatureSensor::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void TemperatureSensor::Read()
{

}

//=====[Implementations of private functions]==================================

} // namespace Drivers
