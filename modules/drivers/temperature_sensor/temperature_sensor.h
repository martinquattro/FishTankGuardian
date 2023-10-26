#ifndef _TEMPERATURE_SENSOR_H
#define _TEMPERATURE_SENSOR_H

namespace Drivers { 

//=====[Declaration of public defines]=========================================
//=====[Declaration of public data types]======================================
//=====[Declarations (prototypes) of public functions]=========================

    class TemperatureSensor 
    {
        public:

            //! Initialize the sensor
            static void Init(int pin);

            //! Returns the sensor object
            static TemperatureSensor* GetInstance();

            //!
            void ReadTemperature();

        private:

            TemperatureSensor(int pin) : mPin(pin) {}
            ~TemperatureSensor() = default;
            TemperatureSensor(const TemperatureSensor&) = delete;
            TemperatureSensor& operator=(const TemperatureSensor&) = delete;

            int mPin;
            static TemperatureSensor* mInstance;
    };
}

#endif // _TEMPERATURE_SENSOR_H