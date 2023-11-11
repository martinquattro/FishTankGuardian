/*!****************************************************************************
 * @file motor.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef MOTOR_H
#define MOTOR_H

namespace Drivers { 

    class Motor 
    {
        public:

            //! Initialize the sensor
            static void Init();

            //! Returns the sensor object
            static Motor* GetInstance();

            //!
            void Read();

        private:

            Motor() {}
            ~Motor() = default;
            Motor(const Motor&) = delete;
            Motor& operator=(const Motor&) = delete;

            static Motor* mInstance;
    };

} // namespace Drivers

#endif // MOTOR_H