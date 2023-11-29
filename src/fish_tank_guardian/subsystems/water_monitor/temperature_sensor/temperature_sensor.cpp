/*!****************************************************************************
 * @file temperature_sensor.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "temperature_sensor.h"

namespace Drivers {

//=====[Declaration and initialization of private global variables]============

TemperatureSensor* TemperatureSensor::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void TemperatureSensor::Init()
{
    if (mInstance == nullptr)
    {
        mInstance = new TemperatureSensor(TEMP_SENSOR_PIN);
    }
}

//----static-------------------------------------------------------------------
TemperatureSensor* TemperatureSensor::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void TemperatureSensor::Update()
{
    float analogReading = 25.0;

    // add reading to vector
    (*mReadingsVectorIter) = analogReading;

    // check if we were at last element of the readings vector
    if (mReadingsVectorIter++ >= mReadingsVector.end())
    {
        mReadingsVectorIter = mReadingsVector.begin();
    }

    // obtaing average
    float tempReadingSum = 0.0;
    int amounfOfReadings = 0;
    for (TempReadingsVec::iterator it = mReadingsVector.begin(); (it != mReadingsVector.end()) ; ++it) 
    {
        if ((*it) > 0.0)
        {
            tempReadingSum = tempReadingSum + (*it);
            ++amounfOfReadings;
        }
    }
    float avgAnalogReading = (tempReadingSum / amounfOfReadings);

    // logic to transform reading to celcius degrees
    const float tempValue = avgAnalogReading;

    // check if we are out of boundaries
    if (tempValue < MIN_TEMP_VALUE)
    {
        mLastReading = MIN_TEMP_VALUE;
    }
    else if (tempValue > MAX_TEMP_VALUE)
    {
        mLastReading = MAX_TEMP_VALUE;
    }
    else
    {
        mLastReading = tempValue;
    }
}

//-----------------------------------------------------------------------------
float TemperatureSensor::GetLastReading()
{
    return mLastReading;
}

//=====[Implementations of private functions]==================================

TemperatureSensor::TemperatureSensor(const PinName pin)
    : mPin(pin)
    , mReadingsVector(TEMP_SENSOR_NUM_AVG_SAMPLES, -1.0)
    , mReadingsVectorIter(mReadingsVector.begin())
{
    mLastReading = 0.0;
}

} // namespace Drivers
