/*!****************************************************************************
 * @file motor.h
 * @brief TODO
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

            //! Initialize the module
            static void Init();

            //! Returns the module object
            static Motor* GetInstance();
           
            //! Start motor rotation
            void Rotate();

        private:

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

            //! Rotate motor by specified number of steps
            void _Rotate(const int numSteps);

            //! De-energize the motor
            void _Denergize();

            Motor(PinName pin1, PinName pin2, PinName pin3, PinName pin4, const int speed);
            ~Motor() = default;
            Motor(const Motor&) = delete;
            Motor& operator=(const Motor&) = delete;

            static Motor* mInstance;

            const int mSpeed;
            DigitalOut mPin1;
            DigitalOut mPin2;
            DigitalOut mPin3;
            DigitalOut mPin4;
    };

} // namespace Drivers

#endif // MOTOR_H