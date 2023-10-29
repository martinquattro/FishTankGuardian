/*!****************************************************************************
 * @file non_blocking_delay.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef NON_BLOCKING_DELAY_H
#define NON_BLOCKING_DELAY_H

//=====[Libraries]=============================================================

#include "mbed.h"

namespace Util { 

    typedef uint64_t tick_t;        

    class NonBlockingDelay 
    {

        public:


            //! Initialize the delay
            static void Init(tick_t duration);

            //! Returns the sensor object
            static NonBlockingDelay* GetInstance();

            //! Check if the delay has finished
            bool HasFinished();

            //! Write a new duration
            void Write(tick_t duration);

        private:

            NonBlockingDelay(tick_t duration) 
                : mDuration(duration) 
                , mStartTime(0)
                , mIsRunning(false)
                {}
            ~NonBlockingDelay() = default;
            NonBlockingDelay(const NonBlockingDelay&) = delete;
            NonBlockingDelay& operator=(const NonBlockingDelay&) = delete;

            tick_t mStartTime;
            tick_t mDuration;
            bool mIsRunning; 
            static NonBlockingDelay* mInstance;
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
            Tick& operator=(const NonBlockingDelay&) = delete;

            static tick_t mTickCounter;  
            static Ticker mTicker;
    };

} // namespace Util

#endif // NON_BLOCKING_DELAY_H