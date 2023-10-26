//=====[Libraries]=============================================================

#include "mbed.h"
#include "temperature_sensor.h"

namespace Drivers {

//=====[Declaration of private defines]========================================
//=====[Declaration of private data types]=====================================
//=====[Declaration and initialization of public global objects]===============
//=====[Declaration of external public global variables]=======================
//=====[Declaration and initialization of public global variables]=============
//=====[Declaration and initialization of private global variables]============

TemperatureSensor* TemperatureSensor::mInstance = nullptr;

//=====[Declarations (prototypes) of private functions]========================
//=====[Implementations of public functions]===================================

//----------------------------------------------------------------
void TemperatureSensor::Init(int pin)
{
    if (mInstance == nullptr)
    {
        mInstance = new TemperatureSensor(pin);
    }
}

//----------------------------------------------------------------
TemperatureSensor* TemperatureSensor::GetInstance()
{
    return mInstance;
}


//----------------------------------------------------------------
void ReadTemperature()
{

}

//=====[Implementations of private functions]==================================

} // namespace Drivers
