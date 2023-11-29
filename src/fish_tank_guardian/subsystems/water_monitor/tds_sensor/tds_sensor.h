/*!****************************************************************************
 * @file tds_sensor.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef TDS_SENSOR_H
#define TDS_SENSOR_H

#include "mbed.h"
#include <vector>

#define TDS_SENSOR_PIN A0
#define TDS_SENSOR_NUM_AVG_SAMPLES 20
#define MIN_TDS_VALUE  0.0
#define MAX_TDS_VALUE  999

namespace Drivers { 

    class TdsSensor 
    {
        public:

            //! Initialize the sensor
            static void Init();

            //! Returns the sensor object
            static TdsSensor* GetInstance();

            //!
            void Update(const float temperature = 25.0);

            //!
            int GetLastReading();

        private:

            using TdsReadingsVec = vector<float>;

            TdsSensor(const PinName pin);
            ~TdsSensor() = default;
            TdsSensor(const TdsSensor&) = delete;
            TdsSensor& operator=(const TdsSensor&) = delete;

            static TdsSensor* mInstance;
            AnalogIn mPin;
            TdsReadingsVec mReadingsVector;
            TdsReadingsVec::iterator mReadingsVectorIter;
            float mTemperature;
            float mRef;
            float mAdcRange;
            float mKValue;
            int   mLastReading;
    };

} // namespace Drivers

#endif // TDS_SENSOR_H