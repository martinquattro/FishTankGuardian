/*!****************************************************************************
 * @file wifi.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "wifi.h"

namespace Drivers {

//=====[Declaration and initialization of private global variables]============

Wifi* Wifi::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void Wifi::Init()
{
    if (mInstance == nullptr)
    {
        mInstance = new Wifi(WIFI_PIN_TX, WIFI_PIN_RX, WIFI_BAUD_RATE);
    }

}

//----static-------------------------------------------------------------------
Wifi* Wifi::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void Wifi::Update()
{

}


//=====[Implementations of private functions]==================================

//-----------------------------------------------------------------------------
Wifi::Wifi(PinName txPin, PinName rxPin, const int baudRate)
    : mSerial(txPin, rxPin, baudRate)
{
    
}

//-----------------------------------------------------------------------------
void Wifi::SerialWrite(std::string str)
{
    mSerial.write(str.c_str(), str.size());
}

} // namespace Drivers
