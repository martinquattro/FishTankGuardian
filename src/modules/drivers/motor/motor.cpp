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
        mInstance = new Motor(MOTOR_PIN_1, MOTOR_PIN_2, MOTOR_PIN_3, MOTOR_PIN_4, MOTOR_SPEED, MOTOR_TOTAL_STEPS);
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
    if (mState == MOTOR_STATE::ROTATING)
    {
        if (mCurrentStep < mNumberSteps)
        {
            _Step();
            mState = MOTOR_STATE::STOPPED;
        }
        else
        {
            mCurrentStep = 0;
            mState = MOTOR_STATE::STOPPED;
        }
    }
}

//-----------------------------------------------------------------------------
void Motor::Start()
{
    mState = MOTOR_STATE::ROTATING;
}

//=====[Implementations of private functions]==================================

//-----------------------------------------------------------------------------
Motor::Motor(PinName pin1, PinName pin2, PinName pin3, PinName pin4, const int speed, const int steps)
    : mPin1(pin1)
    , mPin2(pin2)
    , mPin3(pin3)
    , mPin4(pin4)
    , mSpeed(speed)
    , mNumberSteps(steps)
{
    mState = MOTOR_STATE::ROTATING;
    mCurrentStep = 0;
}

//-----------------------------------------------------------------------------
void Motor::_Step()
{
    // for (int i = 0; i < 4; i++)
    // {
    //     mPin1 = stepsMap[i][0];
    //     mPin2 = stepsMap[i][1];
    //     mPin3 = stepsMap[i][2];
    //     mPin4 = stepsMap[i][3];

    //     ++mCurrentStep;
    //     wait_us(mSpeed);
    // }

    int count = 0; // initalize step count
    while (count < 512)
    {
        for (int i = 7; i >= 0; i--) 
        {
            switch (i) 
            {
                case 0: {
                    mPin1=0;
                    mPin2=0;
                    mPin3=0;
                    mPin4=1;
                }
                break;
                case 1: {
                    mPin1=0;
                    mPin2=0;
                    mPin3=1;
                    mPin4=1;
                }
                break;
                case 2: {
                    mPin1=0;
                    mPin2=0;
                    mPin3=1;
                    mPin4=0;
                }
                break;
                case 3: {
                    mPin1=0;
                    mPin2=1;
                    mPin3=1;
                    mPin4=0;
                }
                break;
                case 4: {
                    mPin1=0;
                    mPin2=1;
                    mPin3=0;
                    mPin4=0;
                }
                break;
                case 5: {
                    mPin1=1;
                    mPin2=1;
                    mPin3=0;
                    mPin4=0;
                }
                break;
                case 6: {
                    mPin1=1;
                    mPin2=0;
                    mPin3=0;
                    mPin4=0;
                }
                break;
                case 7: {
                    mPin1=1;
                    mPin2=0;
                    mPin3=0;
                    mPin4=1;
                }
                break;
            }
            
            wait_us(mSpeed); // wait time defines the speed 
        } 

        count++;
    }
} // namespace Drivers

}