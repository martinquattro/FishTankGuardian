/*!****************************************************************************
 * @file food_feeder.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef FOOD_FEEDER_H
#define FOOD_FEEDER_H

#include <vector>
#include "mbed.h"
#include <string>

#define FEED_TIMES_EEPROM_START     0
#define FEED_TIMES_SLOTS_NUM        5
#define FEED_TIME_BYTES             32      // HH:MM:SS-X where X is the amount of feeds
#define MAX_NUM_FEEDS               3

namespace Subsystems { 

    class FoodFeeder 
    {
        public:

            struct FeedTimeInfo
            {
                std::string mFeedTime;
                int mNumFeeds;
                int mSlot;
            };

            //! Initialize the sensor
            static void Init();

            //! Returns the subsystem  object
            static FoodFeeder* GetInstance();

            //! Update subsystem
            void Update();

            //!
            bool Feed(const int numFeeds);

            //! Add a new time to feed
            bool AddFeedTime(std::string newFeedTime, const int numFeeds, const int feedTimeSlot);

            //!
            bool EraseFeedTime(const int feedTimeSlot);

            //!
            std::string GetNextFeedTime();

            //! Read feed times currently saved in memory
            std::vector<FeedTimeInfo> GetFeedTimes();

        private:

            FoodFeeder();
            ~FoodFeeder() = default;
            FoodFeeder(const FoodFeeder&) = delete;
            FoodFeeder& operator=(const FoodFeeder&) = delete;

            //!
            bool _IsTimeToFeed(std::string currentTime, int* numFeeds);

            // Function to validate the time format
            bool _IsValidTimeFormat(const char* time);

            static FoodFeeder* mInstance;
    };

} // namespace Subsystems

#endif // FOOD_FEEDER_H