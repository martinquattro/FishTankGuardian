/*!****************************************************************************
 * @file tds_sensor.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "tds_sensor.h"

#include "arm_book_lib.h"

namespace Drivers {

//=====[Declaration and initialization of private global variables]============

TdsSensor* TdsSensor::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void TdsSensor::Init()
{
    if (mInstance == nullptr)
    {
        mInstance = new TdsSensor(TDS_SENSOR_PIN);
    }

}

//----static-------------------------------------------------------------------
TdsSensor* TdsSensor::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void TdsSensor::Update(const float temperature /* = 25.0*/)
{
    const float analogReading = mPin.read();

    // add reading to vector
    (*mReadingsVectorIter) = analogReading;

    // check if we were at last element of the readings vector
    if (mReadingsVectorIter++ >= mReadingsVector.end())
    {
        mReadingsVectorIter = mReadingsVector.begin();
    }

    // obtaing average
    float tdsReadingSum = 0.0;
    int amounfOfReadings = 0;
    for (TdsReadingsVec::iterator it = mReadingsVector.begin(); (it != mReadingsVector.end()) ; ++it) 
    {
        if ((*it) > 0.0)
        {
            tdsReadingSum = tdsReadingSum + (*it);
            ++amounfOfReadings;
        }
    }
    float avgAnalogReading = (tdsReadingSum / amounfOfReadings);

    // logic to transform reading to ppm units
    const float voltage = avgAnalogReading * mRef;
    const float ecValue = ((133.42 * voltage * voltage * voltage) - (255.86 * voltage * voltage) + (857.39 * voltage)) * mKValue;
    const float ecValue25 = ecValue / (1.0 + 0.02 * (temperature - 25.0));  //temperature compensation
    const int tdsValue = ecValue25 * 0.5;

    // check if we are out of boundaries
    if (tdsValue < MIN_TDS_VALUE)
    {
        mLastReading = MIN_TDS_VALUE;
    }
    else if (tdsValue > MAX_TDS_VALUE)
    {
        mLastReading = MAX_TDS_VALUE;
    }
    else
    {
        mLastReading = tdsValue;
    }
}

//-----------------------------------------------------------------------------
int TdsSensor::GetLastReading()
{
    return mLastReading;
}

//=====[Implementations of private functions]==================================

TdsSensor::TdsSensor(const PinName pin)
    : mPin(pin, 1024.0)
    , mReadingsVector(TDS_SENSOR_NUM_AVG_SAMPLES, -1.0)
    , mReadingsVectorIter(mReadingsVector.begin())
{
    mTemperature = 25.0;
    mAdcRange = 1024.0;
    mRef = 5.0;
    mKValue = 1.0;
}

} // namespace Drivers
