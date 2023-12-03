/*!****************************************************************************
 * @file motor.h
 * @brief   Declaration of Motor class for 5-12V 28BYJ Stepper Motor with ULN2003 Driver
 * @details This class controls a 12V 28BYJ stepper motor using the ULN2003 driver.
 *          It includes initialization, rotation control, and utility functions.
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"

#define MOTOR_PIN_1         D9
#define MOTOR_PIN_2         D8
#define MOTOR_PIN_3         D7
#define MOTOR_PIN_4         D6
#define MOTOR_TOTAL_STEPS   512
#define MOTOR_SPEED         800

namespace Drivers { 

    class Motor 
    {
        public:

            /**
            * @brief Initialize the motor module.
            */
            static void Init();

            /**
            * @brief Get the instance of the Motor class.
            * @return Pointer to the Motor instance.
            */
            static Motor* GetInstance();
            
            /**
            * @brief Rotate the motor by a specified number of full rotations.
            * @param numFullRotations The number of full rotations.
            */
            void Rotate(const int numFullRotations);

        private:

            /**
            * @brief Constructor for Motor class.
            * @param pin1 Pin for motor control.
            * @param pin2 Pin for motor control.
            * @param pin3 Pin for motor control.
            * @param pin4 Pin for motor control.
            * @param speed Speed of motor rotation.
            */
            Motor(PinName pin1, PinName pin2, PinName pin3, PinName pin4, const int speed);
            
            ~Motor() = default;
            Motor(const Motor&) = delete;
            Motor& operator=(const Motor&) = delete;

            /**
            * @brief Rotate the motor by a specified number of steps.
            * @param numSteps Number of steps to rotate.
            */
            void _Rotate(const int numSteps);

            /**
            * @brief De-energize the motor.
            */
            void _Denergize();

            static Motor* mInstance;
            const int  mSpeed;
            DigitalOut mPin1;
            DigitalOut mPin2;
            DigitalOut mPin3;
            DigitalOut mPin4;

            static constexpr int sStepsMap[8][4] =
            {
                {0, 0, 0, 1},
                {0, 0, 1, 1},
                {0, 0, 1, 0},
                {0, 1, 1, 0},
                {0, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 1},
            };
    };
} // namespace Drivers

#endif // MOTOR_H