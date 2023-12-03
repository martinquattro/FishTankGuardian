/*!****************************************************************************
 * @file water_monitor.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef WATER_MONITOR_H
#define WATER_MONITOR_H

#define TEMP_LIMITS_EEPROM_START        5 * RTC_EEPROM_PAGE_SIZE
#define TDS_LIMITS_EEPROM_START         6 * RTC_EEPROM_PAGE_SIZE
#define MIN_TEMP                        0
#define MAX_TEMP                        85

namespace Subsystems { 

    class WaterMonitor 
    {
        public:

            //! Initialize the sensor
            static void Init();

            //! Returns the subsystem object
            static WaterMonitor* GetInstance();

            //! Update subsystem
            void Update();

            //!
            bool GetWaterState();

            //! 
            int GetTdsReading();

            //!
            float GetTempReading();

            //!
            bool SetTemperatureLimits(const int lowerLimit, const int upperLimit);

            //!
            bool SetTdsLimits(const int lowerLimit, const int upperLimit);

            //!
            bool GetTemperatureLimits(int* lowerLimit, int* upperLimit);

            //!
            bool GetTdsLimits(int* lowerLimit, int* upperLimit);

        private:

            WaterMonitor() {}
            ~WaterMonitor() = default;
            WaterMonitor(const WaterMonitor&) = delete;
            WaterMonitor& operator=(const WaterMonitor&) = delete;

            static WaterMonitor* mInstance;

            //!
            bool _GetSensorLimits(const int eepromPosition, int* lowerLimit, int* upperLimit);

    };

} // namespace Subsystems

#endif // WATER_MONITOR_H