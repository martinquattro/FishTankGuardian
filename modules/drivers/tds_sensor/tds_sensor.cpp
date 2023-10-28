/*!****************************************************************************
 * @file tds_sensor.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "tds_sensor.h"

namespace Drivers {

//=====[Declaration and initialization of private global variables]============

TdsSensor* TdsSensor::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------------
void TdsSensor::Init(int pin)
{
    if (mInstance == nullptr)
    {
        mInstance = new TdsSensor(pin);
    }
}

//----static-------------------------------------------------------------------------
TdsSensor* TdsSensor::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void TdsSensor::Read()
{

}

//=====[Implementations of private functions]==================================

} // namespace Drivers
