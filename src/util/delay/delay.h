/*!****************************************************************************
 * @file delay.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef NON_BLOCKING_DELAY_H
#define NON_BLOCKING_DELAY_H

//=====[Libraries]=============================================================

#include "mbed.h"

#define DELAY_10_SECONDS        10000
#define DELAY_5_SECONDS         5000
#define DELAY_2_SECONDS         2000
#define DELAY_1_SECONDS         1000

namespace Util { 

    typedef uint64_t tick_t;        

    class Delay 
    {

        public:

            //! Check if the delay has finished
            bool HasFinished();

            //! Write a new duration
            void Start(tick_t duration);

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
            bool mIsRunning; 
            Delay* mInstance;
    };

    class Tick
    {
        public:

            //! Initialize the tick
            static void Init();

            //!
            static void TickerCallback();

            //! Return the tick counter
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