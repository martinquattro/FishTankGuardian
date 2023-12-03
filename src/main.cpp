/*!****************************************************************************
 * @file    main.cpp
 * @brief   Main file for the Fish Tank Guardian application.
 * @author  Quattrone Martin
 * @date    Oct 2023
 ******************************************************************************/

#include "fish_tank_guardian.h"

//=====[Implementations of public functions]===================================

//-----------------------------------------------------------------------------
int main()
{
    Subsystems::FishTankGuardian::Init();

    while (true) 
    {
        Subsystems::FishTankGuardian::GetInstance()->Update();
    }
}