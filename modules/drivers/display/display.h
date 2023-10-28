/*!****************************************************************************
 * @file tds_sensor.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

namespace Drivers { 

    class Display 
    {
        public:

            //! Initialize the sensor
            static void Init(int pin);

            //! Returns the sensor object
            static Display* GetInstance();

            //!
            void Read();

        private:

            Display(int pin) : mPin(pin) {}
            ~Display() = default;
            Display(const Display&) = delete;
            Display& operator=(const Display&) = delete;

            int mPin;
            static Display* mInstance;
    };

} // namespace Drivers

#endif // DISPLAY_H