/*!****************************************************************************
 * @file temperature_sensor.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#include "mbed.h"
#include "temperature_sensor.h"

namespace Drivers {

//=====[Declaration and initialization of private global variables]============

TemperatureSensor* TemperatureSensor::mInstance = nullptr;

//=====[Implementations of public methods]=====================================

//----static-------------------------------------------------------------------
void TemperatureSensor::Init()
{
    if (mInstance == nullptr)
    {
        mInstance = new TemperatureSensor(TEMP_SENSOR_PIN);
    }

    mInstance->mLastReading = 0.0;
    mInstance->mRef = 5.0;
}

//----static-------------------------------------------------------------------
TemperatureSensor* TemperatureSensor::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void TemperatureSensor::Update()
{
    float analogReading = mPin.read();

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
    const float tempValue = (avgAnalogReading) * MAX_TEMP_VALUE;

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
        mLastReading = std::round(tempValue * 2) / 2;
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
}

} // namespace Drivers
