/*!****************************************************************************
 * @file pc_serial_com.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "pc_serial_com.h"

#include <cstdarg>
#include "mbed.h"

namespace Util {

//=====[Declaration and initialization of private global variables]============

PcSerialCom* PcSerialCom::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void PcSerialCom::Init()
{
    if (mInstance == nullptr)
    {
        mInstance = new PcSerialCom(USBTX, USBRX, BAUD_RATE);
    }
}

//=====[Implementations of private functions]==================================

PcSerialCom::PcSerialCom(PinName txPin, PinName rxPin, const int baudRate)
    : mUartUsb(txPin, rxPin, baudRate)
{
}

} // namespace Util