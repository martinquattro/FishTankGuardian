/*!****************************************************************************
 * @file delay.h
 * @brief Declaration of non-blocking delay module methods and definitions
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef NON_BLOCKING_DELAY_H
#define NON_BLOCKING_DELAY_H

#include "mbed.h"

#define DELAY_5_MINUTES         300000
#define DELAY_10_SECONDS        10000
#define DELAY_5_SECONDS         5000
#define DELAY_4_SECONDS         4000
#define DELAY_3_SECONDS         3000
#define DELAY_2_SECONDS         2000
#define DELAY_1_SECONDS         1000

namespace Util { 

    typedef uint64_t tick_t;

    class Delay 
    {
        public:

            /**
            * @brief Check if the delay has finished.
            * @return True if the delay has finished, false otherwise.
            */
            bool HasFinished();

            /**
            * @brief Write a new duration for the delay.
            * @param duration The new duration for the delay.
            */
            void Start(tick_t duration);

            /**
            * @brief Constructor for Delay class.
            * @param duration Initial duration for the delay.
            */
            Delay(tick_t duration) 
                : mDuration(duration) 
                , mStartTime(0)
                , mIsRunning(false)
                {}
            
            ~Delay() = default;
            Delay(const Delay&) = delete;
            Delay& operator=(const Delay&) = delete;

        private:

            tick_t mStartTime;
            tick_t mDuration;
            bool   mIsRunning; 
            Delay* mInstance;
    };

    class Tick
    {
        public:

            /**
            * @brief Initialize the tick.
            */
            static void Init();

            /**
            * @brief Callback function for the ticker.
            */
            static void TickerCallback();

            /**
            * @brief Return the tick counter.
            * @return The current tick counter.
            */
            static tick_t GetTickCounter();

        private:

            Tick() {};
            ~Tick() = default;
            Tick(const Tick&) = delete;
            Tick& operator=(const Delay&) = delete;

            static tick_t mTickCounter;  
            static Ticker mTicker;
    };

} // namespace Util

#endif // NON_BLOCKING_DELAY_H