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
#define MOTOR_TOTAL_STEPS     2048
#define MOTOR_SPEED           1200

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
                STOPPED,
                ROTATING
            };

            int stepsMap[4][4] =
            {
                {1, 0, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1}
            };

            //!
            void _Step();

            Motor(PinName pin1, PinName pin2, PinName pin3, PinName pin4, const int speed, const int steps);
            ~Motor() = default;
            Motor(const Motor&) = delete;
            Motor& operator=(const Motor&) = delete;

            static Motor* mInstance;

            const int mNumberSteps;
            const int mSpeed;

            MOTOR_STATE mState;
            int mCurrentStep;
            DigitalOut mPin1;
            DigitalOut mPin2;
            DigitalOut mPin3;
            DigitalOut mPin4;
    };

} // namespace Drivers

#endif // MOTOR_H