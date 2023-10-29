/*!****************************************************************************
 * @file user_interface.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================


#include "user_interface.h"

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
}

//----static-------------------------------------------------------------------
UserInterface* UserInterface::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void UserInterface::Update()
{
    // TODO: Update Drivers controlled by the UserInterface Subsystem
}

//=====[Implementations of private functions]==================================

} // namespace Subsystems
