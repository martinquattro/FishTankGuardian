/*!****************************************************************************
 * @file food_feeder.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef FOOD_FEEDER_H
#define FOOD_FEEDER_H

#include <vector>
// #include "EEPROM.h"
#include "mbed.h"

namespace Subsystems { 

    class FoodFeeder 
    {
        public:

            //! Initialize the sensor
            static void Init();

            //! Returns the subsystem  object
            static FoodFeeder* GetInstance();

            //! Update subsystem
            void Update();

            //! Add a new time to feed
            bool AddFeedTime(std::string newFeedTime);

            //! Read feed times currently saved in memory
            std::vector<std::string> GetFeedTimes();

        private:

            FoodFeeder();
            ~FoodFeeder() = default;
            FoodFeeder(const FoodFeeder&) = delete;
            FoodFeeder& operator=(const FoodFeeder&) = delete;

            //!
            bool _IsTimeToFeed(std::string currentTime);

            // Function to validate the time format
            bool _IsValidTimeFormat(const char* time);

            static FoodFeeder* mInstance;
    };

} // namespace Subsystems

#endif // FOOD_FEEDER_H