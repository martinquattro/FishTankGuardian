/*!****************************************************************************
 * @file food_feeder.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "food_feeder.h"
#include "motor.h"


namespace Subsystems {

//=====[Declaration and initialization of private global variables]============

FoodFeeder* FoodFeeder::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void FoodFeeder::Init()
{
    if (mInstance == nullptr)
    {
        mInstance = new FoodFeeder();
    }

    Drivers::Motor::Init();
}

//----static-------------------------------------------------------------------
FoodFeeder* FoodFeeder::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void FoodFeeder::Update()
{
    Drivers::Motor::GetInstance()->Update();
}

//=====[Implementations of private functions]==================================

} // namespace Subsystems
