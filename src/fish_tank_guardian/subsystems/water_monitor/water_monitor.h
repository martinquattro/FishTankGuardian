/*!****************************************************************************
 * @file water_monitor.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef WATER_MONITOR_H
#define WATER_MONITOR_H

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
            int GetTdsReading();

            //!
            float GetTempReading();

        private:

            WaterMonitor() {}
            ~WaterMonitor() = default;
            WaterMonitor(const WaterMonitor&) = delete;
            WaterMonitor& operator=(const WaterMonitor&) = delete;

            static WaterMonitor* mInstance;
    };

} // namespace Subsystems

#endif // WATER_MONITOR_H