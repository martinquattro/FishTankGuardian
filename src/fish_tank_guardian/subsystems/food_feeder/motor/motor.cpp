/*!****************************************************************************
 * @file    motor.cpp
 * @brief   Implementation of the Motor class for 5-12V 28BYJ Stepper Motor with ULN2003 Driver
 * @details This code controls a 12V 28BYJ stepper motor using the ULN2003 driver.
 *          It includes initialization, rotation control, and utility functions.
 * @author  Quattrone Martin
 * @date    Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "motor.h"
#include "arm_book_lib.h"

namespace Drivers {

//=====[Declaration and initialization of private global variables]============

Motor* Motor::mInstance = nullptr;

//=====[Implementations of public methods]=====================================

//----static-------------------------------------------------------------------
void Motor::Init()
{
    if (mInstance == nullptr)
    {
        mInstance = new Motor(MOTOR_PIN_1, MOTOR_PIN_2, MOTOR_PIN_3, MOTOR_PIN_4, MOTOR_SPEED);
    }

    mInstance->_Denergize();
}

//----static-------------------------------------------------------------------
Motor* Motor::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void Motor::Rotate(const int numFullRotations)
{
    _Rotate(numFullRotations * MOTOR_TOTAL_STEPS);
    _Denergize();
}

//=====[Implementations of private methods]====================================

//----private------------------------------------------------------------------
Motor::Motor(PinName pin1, PinName pin2, PinName pin3, PinName pin4, const int speed)
    : mPin1(pin1)
    , mPin2(pin2)
    , mPin3(pin3)
    , mPin4(pin4)
    , mSpeed(speed)
{
}

//----private------------------------------------------------------------------
void Motor::_Denergize()
{
    mPin1.write(LOW);
    mPin2.write(LOW);
    mPin3.write(LOW);
    mPin4.write(LOW);
}

//----private------------------------------------------------------------------
void Motor::_Rotate(const int numSteps)
{
    int count = 0;
    while (count++ < numSteps)
    {
        for (int i = 0; i < 8; ++i)
        {
            mPin1 = sStepsMap[i][0];
            mPin2 = sStepsMap[i][1];
            mPin3 = sStepsMap[i][2];
            mPin4 = sStepsMap[i][3];

            wait_us(mSpeed);
        }
    }
} // namespace Drivers

}