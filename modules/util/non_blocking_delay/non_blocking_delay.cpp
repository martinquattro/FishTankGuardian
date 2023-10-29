/*!****************************************************************************
 * @file non_blocking_delay.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#include "non_blocking_delay.h"

namespace Util { 

//=====[Declaration and initialization of private global variables]============

NonBlockingDelay* NonBlockingDelay::mInstance = nullptr;
tick_t Tick::mTickCounter = 0;
Ticker Tick::mTicker;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void NonBlockingDelay::Init(tick_t duration)
{
    if (mInstance == nullptr)
    {
        mInstance = new NonBlockingDelay(duration);
    }
}

//----static-------------------------------------------------------------------
NonBlockingDelay* NonBlockingDelay::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
bool NonBlockingDelay::HasFinished()
{
    bool timeArrived = false;
    tick_t elapsedTime;

    if(mIsRunning) 
    {
        mStartTime = Tick::GetTickCounter();
        mIsRunning = true;
    } 
    else 
    {
      elapsedTime = Tick::GetTickCounter() - mStartTime;
      if (elapsedTime >= mDuration) 
        {
            timeArrived = true;
            mIsRunning = false;
        }
    }

   return timeArrived;
}

//-----------------------------------------------------------------------------
void NonBlockingDelay::Write(tick_t durationValue )
{
   mDuration = durationValue;
}

//----static-------------------------------------------------------------------
void Tick::Init() 
{
    mTicker.attach(TickerCallback, 1ms);
}

//----static-------------------------------------------------------------------
void Tick::TickerCallback() 
{
    mTickCounter++;
}

//----static-------------------------------------------------------------------
tick_t Tick::GetTickCounter() 
{
    return mTickCounter;
}

} // namespace Util
