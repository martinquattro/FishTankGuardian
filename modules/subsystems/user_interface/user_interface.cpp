/*!****************************************************************************
 * @file user_interface.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "user_interface.h"
#include "wifi.h"

namespace Subsystems {

//=====[Declaration and initialization of private global variables]============

UserInterface* UserInterface::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void UserInterface::Init()
{
    if (mInstance == nullptr)
    {
        mInstance = new UserInterface();
    }

    Drivers::Wifi::Init();

}

//----static-------------------------------------------------------------------
UserInterface* UserInterface::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void UserInterface::Update()
{
    Drivers::Wifi::GetInstance()->Update();
}

//=====[Implementations of private functions]==================================

} // namespace Subsystems
