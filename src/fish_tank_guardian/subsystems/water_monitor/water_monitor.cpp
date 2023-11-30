/*!****************************************************************************
 * @file water_monitor.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "water_monitor.h"

#include "arm_book_lib.h"
#include "temperature_sensor.h"
#include "tds_sensor.h"
#include "real_time_clock.h"
#include "utilities.h"

namespace Subsystems {

//=====[Declaration and initialization of private global variables]============

WaterMonitor* WaterMonitor::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void WaterMonitor::Init()
{
    DEBUG_PRINT("WaterMonitor - Initiating...\r\n");

    if (mInstance == nullptr)
    {
        mInstance = new WaterMonitor();
    }

    Drivers::TemperatureSensor::Init();
    Drivers::TdsSensor::Init();

    mInstance->SetTemperatureLimits(static_cast<int>(MIN_TEMP_VALUE), static_cast<int>(MAX_TEMP_VALUE));
    mInstance->SetTdsLimits(static_cast<int>(MIN_TDS_VALUE), static_cast<int>(MAX_TDS_VALUE));

    DEBUG_PRINT("UserInterface - [OK] Initialized\r\n");
}

//----static-------------------------------------------------------------------
WaterMonitor* WaterMonitor::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void WaterMonitor::Update()
{
    Drivers::TemperatureSensor::GetInstance()->Update();
    Drivers::TdsSensor::GetInstance()->Update(GetTempReading());            // tds sensor uses temperature reading to have a better measure
}

//-----------------------------------------------------------------------------
bool WaterMonitor::GetWaterState()
{
    int tempLowerLimit, tempUpperLimit, tdsLowerLimit, tdsUpperLimit;
    const int tdsReading = GetTdsReading();
    const int tempReading = GetTempReading();

    if (GetTemperatureLimits(&tempLowerLimit, &tempUpperLimit))
    {
        if (tempReading < tempLowerLimit || tempReading > tempUpperLimit)
        {
            return false;
        }
    }

    if (GetTdsLimits(&tdsLowerLimit, &tdsUpperLimit))
    {
        if (tdsReading < tdsLowerLimit || tdsReading > tdsUpperLimit)
        {
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
int WaterMonitor::GetTdsReading()
{
    return (Drivers::TdsSensor::GetInstance()->GetLastReading());
}

//-----------------------------------------------------------------------------
float WaterMonitor::GetTempReading()
{
    return (Drivers::TemperatureSensor::GetInstance()->GetLastReading());
}

//-----------------------------------------------------------------------------
bool WaterMonitor::SetTemperatureLimits(const int lowerLimit, const int upperLimit)
{
    if (lowerLimit < MIN_TEMP_VALUE || upperLimit > MAX_TEMP_VALUE || lowerLimit > upperLimit) 
    {
        return false;
    }

    int eepromPosition = TEMP_LIMITS_EEPROM_START;
    std::string strToSave = (std::to_string(lowerLimit) + "-" + std::to_string(upperLimit));
    
    // Save the feed time to EEPROM
    Subsystems::RealTimeClock::GetInstance()->SaveStringToEeprom(eepromPosition, strToSave);

    return true;
}

//-----------------------------------------------------------------------------
bool WaterMonitor::SetTdsLimits(const int lowerLimit, const int upperLimit)
{
    if (lowerLimit < MIN_TDS_VALUE || upperLimit > MAX_TDS_VALUE || lowerLimit > upperLimit) 
    {
        return false;
    }

    int eepromPosition = TDS_LIMITS_EEPROM_START;
    std::string strToSave = (std::to_string(lowerLimit) + "-" + std::to_string(upperLimit));
    
    // Save the feed time to EEPROM
    Subsystems::RealTimeClock::GetInstance()->SaveStringToEeprom(eepromPosition, strToSave);

    return true;
}

//-----------------------------------------------------------------------------
bool WaterMonitor::GetTemperatureLimits(int* lowerLimit, int* upperLimit)
{
    return (_GetSensorLimits(TEMP_LIMITS_EEPROM_START, lowerLimit, upperLimit));
}

//-----------------------------------------------------------------------------
bool WaterMonitor::GetTdsLimits(int* lowerLimit, int* upperLimit)
{
    return (_GetSensorLimits(TDS_LIMITS_EEPROM_START, lowerLimit, upperLimit));
}

//-----------------------------------------------------------------------------
bool WaterMonitor::_GetSensorLimits(const int eepromPosition, int* lowerLimit, int* upperLimit)
{   
    // Save the feed time to EEPROM
    std::string limitsStr = Subsystems::RealTimeClock::GetInstance()->ReadStringFromEeprom(eepromPosition);

    size_t dashPos = limitsStr.find('-');
    if (dashPos ==  std::string::npos)
    {
        return false;
    }

    std::string lowerLimitStr = limitsStr.substr(0, dashPos);
    std::string upperLimitStr = limitsStr.substr(dashPos + 1);

    if (Utilities::IsNumeric(lowerLimitStr) && Utilities::IsNumeric(upperLimitStr))
    {
        (*lowerLimit) = stoi(lowerLimitStr);
        (*upperLimit) = stoi(upperLimitStr);

        return true;
    }

    return false;
}

//=====[Implementations of private functions]==================================

} // namespace Subsystems
