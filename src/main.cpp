/*!****************************************************************************
 * @file main.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

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