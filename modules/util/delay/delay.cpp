/*!****************************************************************************
 * @file delay.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#include "delay.h"

namespace Util { 

//=====[Declaration and initialization of private global variables]============

Delay* Delay::mInstance = nullptr;
tick_t Tick::mTickCounter = 0;
Ticker Tick::mTicker;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void Delay::Init(tick_t duration)
{
    if (mInstance == nullptr)
    {
        mInstance = new Delay(duration);
    }
}

//----static-------------------------------------------------------------------
Delay* Delay::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
bool Delay::HasFinished()
{
    bool timeArrived = false;
    tick_t elapsedTime;

    if(!mIsRunning) 
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
void Delay::Write(tick_t durationValue )
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
