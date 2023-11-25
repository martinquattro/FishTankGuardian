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
    (*mReadingsVectorIter) = analogReading;
    // DEBUG_PRINT("analogReading = %f\r\n", analogReading);

    if (mReadingsVectorIter++ >= mReadingsVector.end())
    {
        mReadingsVectorIter = mReadingsVector.begin();
    }

    float tdsReadingSum = 0.0;
    for (TdsReadingsVec::iterator it = mReadingsVector.begin(); (it != mReadingsVector.end()) ; ++it) 
    {
        tdsReadingSum = tdsReadingSum + (*it);
    }

    float avgAnalogReading = (tdsReadingSum / TDS_SENSOR_NUM_AVG_SAMPLES);
	const float voltage = avgAnalogReading * mRef;
	const float ecValue = ((133.42 * voltage * voltage * voltage) - (255.86 * voltage * voltage) + (857.39 * voltage)) * mKValue;
	const float ecValue25 = ecValue / (1.0 + 0.02 * (temperature - 25.0));  //temperature compensation
	const int tdsValue = ecValue25 * 0.5;

    if (tdsValue < 0)
    {
        mLastReading = 0;
    }
    else if (tdsValue > 999)
    {
        mLastReading = 999;
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
    , mReadingsVector(TDS_SENSOR_NUM_AVG_SAMPLES, 0.0)
    , mReadingsVectorIter(mReadingsVector.begin())
{
    mTemperature = 25.0;
    mAdcRange = 1024.0;
    mRef = 5.0;
    mKValue = 1.0;
}

} // namespace Drivers
