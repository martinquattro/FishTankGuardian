/*!****************************************************************************
 * @file motor.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "motor.h"
#include "arm_book_lib.h"

namespace Drivers {

//=====[Declaration and initialization of private global variables]============

Motor* Motor::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void Motor::Init()
{
    if (mInstance == nullptr)
    {
        mInstance = new Motor(MOTOR_PIN_1, MOTOR_PIN_2, MOTOR_PIN_3, MOTOR_PIN_4, MOTOR_SPEED);
    }
}

//----static-------------------------------------------------------------------
Motor* Motor::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void Motor::Update()
{
    if (mState == MOTOR_STATE::ROTATE)
    {
        _Rotate(MOTOR_TOTAL_STEPS);

        _Denergize();
        mState = MOTOR_STATE::STOP;
    }
}

//-----------------------------------------------------------------------------
void Motor::Start()
{
    mState = MOTOR_STATE::ROTATE;
}

//=====[Implementations of private functions]==================================

//-----------------------------------------------------------------------------
Motor::Motor(PinName pin1, PinName pin2, PinName pin3, PinName pin4, const int speed)
    : mPin1(pin1)
    , mPin2(pin2)
    , mPin3(pin3)
    , mPin4(pin4)
    , mSpeed(speed)
{
    _Denergize();

    mState = MOTOR_STATE::ROTATE;
}

//-----------------------------------------------------------------------------
void Motor::_Denergize()
{
    mPin1.write(LOW);
    mPin2.write(LOW);
    mPin3.write(LOW);
    mPin4.write(LOW);
}

//-----------------------------------------------------------------------------
void Motor::_Rotate(const int numSteps)
{
    int count = 0;
    while (count++ < numSteps)
    {
        for (int i = 0; i < 8; ++i)
        {
            mPin1 = stepsMap[i][0];
            mPin2 = stepsMap[i][1];
            mPin3 = stepsMap[i][2];
            mPin4 = stepsMap[i][3];

            wait_us(mSpeed);
        }
    }

} // namespace Drivers

}