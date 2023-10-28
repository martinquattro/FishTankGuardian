/*!****************************************************************************
 * @file display.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "display.h"

namespace Drivers {

//=====[Declaration and initialization of private global variables]============

Display* Display::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void Display::Init(int pin)
{
    if (mInstance == nullptr)
    {
        mInstance = new Display(pin);
    }
}

//----static-------------------------------------------------------------------
Display* Display::GetInstance()
{
    return mInstance;
}

//=====[Implementations of private functions]==================================

} // namespace Drivers
