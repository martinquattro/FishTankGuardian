/*!****************************************************************************
 * @file    food_feeder.h
 * @brief   Declaration of the FoodFeeder class
 * @details This class controls a food feeder system, managing feed times,
 *          feeding operations, and related functionality.
 * @author  Quattrone Martin
 * @date    Oct 2023
 *******************************************************************************/

#ifndef FOOD_FEEDER_H
#define FOOD_FEEDER_H

#include <vector>
#include "mbed.h"
#include <string>

#define FEED_TIMES_EEPROM_START     0
#define FEED_TIMES_SLOTS_NUM        5
#define FEED_TIME_BYTES             32
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

            /**
            * @brief Initialize the food feeder subsystem.
            */
            static void Init();

            /**
            * @brief Get the instance of the FoodFeeder class.
            * @return Pointer to the FoodFeeder instance.
            */
            static FoodFeeder* GetInstance();

            /**
            * @brief Update the food feeder subsystem.
            */
            void Update();

            /**
            * @brief Feed the pets with a specified number of units of food.
            * @param numFeeds Number of units of food to feed.
            * @return True if feeding is successful, false otherwise.
            */
            bool Feed(const int numFeeds);

            /**
            * @brief Erase a feed time in a specified slot.
            * @param feedTimeSlot Slot to erase the feed time.
            * @return True if erasing is successful, false otherwise.
            */
            bool EraseFeedTime(const int feedTimeSlot);

            /**
            * @brief Add a new feed time to the specified slot.
            * @param newFeedTime New feed time in HH:MM:SS format.
            * @param numFeeds Number of units of food to feed.
            * @param feedTimeSlot Slot to add the feed time.
            * @return True if adding is successful, false otherwise.
            */
            bool AddFeedTime(std::string newFeedTime, const int numFeeds, const int feedTimeSlot);

            /**
            * @brief Get the next feed time scheduled.
            * @return The next feed time in HH:MM:SS format. Tomorrow if none today or none set if any
            */
            std::string GetNextFeedTime();

            /**
            * @brief Get the list of feed times.
            * @return Vector of FeedTimeInfo objects representing feed times.
            */
            std::vector<FeedTimeInfo> GetFeedTimes();

        private:

            /**
            * @brief Constructor for FoodFeeder class.
            */
            FoodFeeder();
            
            ~FoodFeeder() = default;
            FoodFeeder(const FoodFeeder&) = delete;
            FoodFeeder& operator=(const FoodFeeder&) = delete;

            /**
            * @brief Validate if the time format is correct (HH:MM:SS).
            * @param time String representing the time.
            * @return True if the time format is correct, false otherwise.
            */
            bool _IsValidTimeFormat(const char* time);

            /**
            * @brief Check if it is time to feed based on the current time.
            * @param currentTime String representing the current time.
            * @param numFeeds Pointer to store the number of feeds.
            * @return True if it is time to feed, false otherwise.
            */
            bool _IsTimeToFeed(std::string currentTime, int* numFeeds);

            static FoodFeeder* mInstance;
    };

} // namespace Subsystems

#endif // FOOD_FEEDER_H