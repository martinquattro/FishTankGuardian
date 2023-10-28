/*!****************************************************************************
 * @file temperature_sensor.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

namespace Drivers { 

    class TemperatureSensor 
    {
        public:

            //! Initialize the sensor
            static void Init(int pin);

            //! Returns the sensor object
            static TemperatureSensor* GetInstance();

            //!
            void Read();

        private:

            TemperatureSensor(int pin) : mPin(pin) {}
            ~TemperatureSensor() = default;
            TemperatureSensor(const TemperatureSensor&) = delete;
            TemperatureSensor& operator=(const TemperatureSensor&) = delete;

            int mPin;
            static TemperatureSensor* mInstance;
    };

} // namespace Drivers

#endif // TEMPERATURE_SENSOR_H