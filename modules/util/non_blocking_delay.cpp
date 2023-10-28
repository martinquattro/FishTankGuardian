//=====[Libraries]=============================================================

#include "non_blocking_delay.h"

//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

namespace Util { 

static Ticker ticker;
static tick_t tickCounter;

//=====[Declarations (prototypes) of private functions]========================

void tickerCallback();
tick_t tickRead();

//=====[Implementations of public functions]===================================

void TickInit()
{
    ticker.attach( tickerCallback, 1ms);
}

void NonBlockingDelayInit( nonBlockingDelay_t * delay, tick_t durationValue )
{
   delay->duration = durationValue;
   delay->isRunning = false;
}

bool NonBlockingDelayRead( nonBlockingDelay_t * delay )
{
   bool timeArrived = false;
   tick_t elapsedTime;

   if( !delay->isRunning ) {
      delay->startTime = tickCounter;
      delay->isRunning = true;
   } else {
      elapsedTime = tickCounter - delay->startTime;
      if ( elapsedTime >= delay->duration ) {
         timeArrived = true;
         delay->isRunning = false;
      }
   }

   return timeArrived;
}

void NonBlockingDelayWrite( nonBlockingDelay_t * delay, tick_t durationValue )
{
   delay->duration = durationValue;
}

//=====[Implementations of private functions]==================================

void tickerCallback( void ) 
{
    tickCounter++;
}

} // namespace Util
