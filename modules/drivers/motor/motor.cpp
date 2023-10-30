/*!****************************************************************************
 * @file motor.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "motor.h"

namespace Drivers {

//=====[Declaration and initialization of private global variables]============

Motor* Motor::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void Motor::Init()
{
    if (mInstance == nullptr)
    {
        mInstance = new Motor();
    }
}

//----static-------------------------------------------------------------------
Motor* Motor::GetInstance()
{
    return mInstance;
}

//=====[Implementations of private functions]==================================

} // namespace Drivers
