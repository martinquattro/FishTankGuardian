/*!****************************************************************************
 * @file user_interface.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "user_interface.h"

#include "arm_book_lib.h"
#include "display.h"

namespace Subsystems {

//=====[Declaration and initialization of private global variables]============

UserInterface* UserInterface::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void UserInterface::Init()
{
    DEBUG_PRINT("UserInterface::Init() - Initiating...\r\n");

    if (mInstance == nullptr)
    {
        mInstance = new UserInterface();
    }

    displayInit(DISPLAY_TYPE_LCD_HD44780, DISPLAY_CONNECTION_GPIO_4BITS);
    displayClear();

    displayCharPositionWrite (0,0);
    displayStringWrite("Fish Tank Guardian");

    displayCharPositionWrite (0,1);
    displayStringWrite("Next Feed:" );

    displayCharPositionWrite (0,2);
    displayStringWrite("Temperature:");
    
    displayCharPositionWrite (0,2);
    displayStringWrite("TDS:" );

    DEBUG_PRINT("UserInterface::Init() - Initiating Finished.\r\n");
}

//----static-------------------------------------------------------------------
UserInterface* UserInterface::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void UserInterface::Update()
{
    
}

//=====[Implementations of private functions]==================================

} // namespace Subsystems
