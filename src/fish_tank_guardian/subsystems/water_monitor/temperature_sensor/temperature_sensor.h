/*!****************************************************************************
 * @file temperature_sensor.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <vector>
#include "mbed.h"

#define TEMP_SENSOR_NUM_AVG_SAMPLES 20
#define TEMP_SENSOR_PIN A1
#define MIN_TEMP_VALUE  0.0
#define MAX_TEMP_VALUE  99.9

namespace Drivers { 

    class TemperatureSensor 
    {
        public:

            //! Initialize the sensor
            static void Init();

            //! Returns the sensor object
            static TemperatureSensor* GetInstance();

            //! 
            void Update();

            //!
            float GetLastReading();

        private:

            using TempReadingsVec = vector<float>;

            TemperatureSensor(const PinName pin);
            ~TemperatureSensor() = default;
            TemperatureSensor(const TemperatureSensor&) = delete;
            TemperatureSensor& operator=(const TemperatureSensor&) = delete;

            static TemperatureSensor* mInstance;
            AnalogIn mPin;
            TempReadingsVec mReadingsVector;
            TempReadingsVec::iterator mReadingsVectorIter;
            float mLastReading;
            float mRef;
    };

} // namespace Drivers

#endif // TEMPERATURE_SENSOR_H