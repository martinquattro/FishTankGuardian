/*!****************************************************************************
 * @file motor.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef MOTOR_H
#define MOTOR_H

#include "mbed.h"

#define MOTOR_PIN_1           D12
#define MOTOR_PIN_2           D13
#define MOTOR_PIN_3           D14
#define MOTOR_PIN_4           D15
#define MOTOR_TOTAL_STEPS     512
#define MOTOR_SPEED           800

namespace Drivers { 

    class Motor 
    {
        public:

            //! Initialize the sensor
            static void Init();

            //! Returns the sensor object
            static Motor* GetInstance();

            //!
            void Update();
            
            //!
            void Start();

        private:

            enum class MOTOR_STATE
            {
                STOP,
                ROTATE
            };

            const int stepsMap[8][4] =
            {
                {0, 0, 0, 1},
                {0, 0, 1, 1},
                {0, 0, 1, 0},
                {0, 1, 1, 0},
                {0, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
            };

            //!
            void _Rotate(const int numSteps);

            //!
            void _Denergize();

            Motor(PinName pin1, PinName pin2, PinName pin3, PinName pin4, const int speed);
            ~Motor() = default;
            Motor(const Motor&) = delete;
            Motor& operator=(const Motor&) = delete;

            static Motor* mInstance;

            const int mSpeed;

            MOTOR_STATE mState;
            DigitalOut mPin1;
            DigitalOut mPin2;
            DigitalOut mPin3;
            DigitalOut mPin4;
    };

} // namespace Drivers

#endif // MOTOR_H