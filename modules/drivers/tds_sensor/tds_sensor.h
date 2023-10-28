/*!****************************************************************************
 * @file tds_sensor.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef TDS_SENSOR_H
#define TDS_SENSOR_H

namespace Drivers { 

    class TdsSensor 
    {
        public:

            //! Initialize the sensor
            static void Init(int pin);

            //! Returns the sensor object
            static TdsSensor* GetInstance();

            //!
            void Read();

        private:

            TdsSensor(int pin) : mPin(pin) {}
            ~TdsSensor() = default;
            TdsSensor(const TdsSensor&) = delete;
            TdsSensor& operator=(const TdsSensor&) = delete;

            int mPin;
            static TdsSensor* mInstance;
    };

} // namespace Drivers

#endif // TDS_SENSOR_H