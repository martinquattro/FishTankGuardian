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

//=====[Declaration of public data types]======================================

typedef uint64_t tick_t;
typedef struct
{
   tick_t startTime;
   tick_t duration;
   bool isRunning;
} nonBlockingDelay_t;

//=====[Declarations (prototypes) of public functions]=========================

void TickInit();

void NonBlockingDelayInit( nonBlockingDelay_t* delay, tick_t durationValue );
bool NonBlockingDelayRead( nonBlockingDelay_t* delay );
void NonBlockingDelayWrite( nonBlockingDelay_t* delay, tick_t durationValue );

} // namespace Util

#endif // NON_BLOCKING_DELAY_H