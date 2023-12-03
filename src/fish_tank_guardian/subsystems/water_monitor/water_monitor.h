/*!****************************************************************************
 * @file    water_monitor.h
 * @brief   WaterMonitor class header file.
 * @author  Quattrone Martin
 * @date    Oct 2023
 ******************************************************************************/

#ifndef WATER_MONITOR_H
#define WATER_MONITOR_H

#define TEMP_LIMITS_EEPROM_START    5 * RTC_EEPROM_PAGE_SIZE
#define TDS_LIMITS_EEPROM_START     6 * RTC_EEPROM_PAGE_SIZE
#define MIN_TEMP                    0
#define MAX_TEMP                    85

namespace Subsystems {

    class WaterMonitor 
    {
        public:

            /*!
            * @brief Initializes the WaterMonitor.
            */
            static void Init();

            /*!
            * @brief Gets the singleton instance of the WaterMonitor.
            * @return WaterMonitor* Pointer to the WaterMonitor instance.
            */
            static WaterMonitor* GetInstance();

            /*!
            * @brief Updates the WaterMonitor readings.
            */
            void Update();

            /*!
            * @brief Gets the water state based on temperature and TDS readings.
            * @return bool True if the water is within acceptable limits, false otherwise.
            */
            bool GetWaterState();

            /*!
            * @brief Gets the last TDS reading.
            * @return int Last TDS reading.
            */
            int GetTdsReading();

            /*!
            * @brief Gets the last temperature reading.
            * @return float Last temperature reading.
            */
            float GetTempReading();

            /*!
            * @brief Sets temperature limits.
            * @param lowerLimit Lower temperature limit.
            * @param upperLimit Upper temperature limit.
            * @return bool True if successful, false otherwise.
            */
            bool SetTemperatureLimits(const int lowerLimit, const int upperLimit);

            /*!
            * @brief Sets TDS limits.
            * @param lowerLimit Lower TDS limit.
            * @param upperLimit Upper TDS limit.
            * @return bool True if successful, false otherwise.
            */
            bool SetTdsLimits(const int lowerLimit, const int upperLimit);

            /*!
            * @brief Gets temperature limits.
            * @param lowerLimit Pointer to store the lower temperature limit.
            * @param upperLimit Pointer to store the upper temperature limit.
            * @return bool True if successful, false otherwise.
            */
            bool GetTemperatureLimits(int* lowerLimit, int* upperLimit);

            /*!
            * @brief Gets TDS limits.
            * @param lowerLimit Pointer to store the lower TDS limit.
            * @param upperLimit Pointer to store the upper TDS limit.
            * @return bool True if successful, false otherwise.
            */
            bool GetTdsLimits(int* lowerLimit, int* upperLimit);

        private:

            WaterMonitor() {}
            ~WaterMonitor() = default;
            WaterMonitor(const WaterMonitor&) = delete;
            WaterMonitor& operator=(const WaterMonitor&) = delete;

            /*!
            * @brief Gets temperature or TDS limits from EEPROM.
            * @param eepromPosition EEPROM position to read.
            * @param lowerLimit Pointer to store the lower limit.
            * @param upperLimit Pointer to store the upper limit.
            * @return bool True if successful, false otherwise.
            */
            bool _GetSensorLimits(const int eepromPosition, int* lowerLimit, int* upperLimit);

            static WaterMonitor* mInstance;
        };

} // namespace Subsystems

#endif // WATER_MONITOR_H
