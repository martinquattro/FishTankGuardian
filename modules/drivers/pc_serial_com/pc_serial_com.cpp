//=====[Libraries]=============================================================
/*!****************************************************************************
 * @file pc_serial_com.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "mbed.h"
#include "pc_serial_com.h"
#include <cstdarg>

namespace Drivers {

//=====[Declaration and initialization of private global variables]============

PcSerialCom* PcSerialCom::mInstance = nullptr;

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void PcSerialCom::Init()
{
    if (mInstance == nullptr)
    {
        mInstance = new PcSerialCom();
    }
}

//----static-------------------------------------------------------------------
PcSerialCom* PcSerialCom::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void PcSerialCom::Write(const char* format, ...)
{
    // Initialize the variable arguments list
    va_list args;
    va_start(args, format);

    // Calculate the length of the formatted string
    int length = vsnprintf(nullptr, 0, format, args);

    // Allocate memory for the formatted string
    char* buffer = new char[length + 1];

    // Format the string and store it in the buffer
    vsnprintf(buffer, length + 1, format, args);

    // Clean up the variable arguments list
    va_end(args);

    // Write the string to the serial port
    mUartUsb.write(buffer, strlen(buffer));

    // Liberar la memoria del búfer
    delete[] buffer;
}

//=====[Implementations of private functions]==================================

PcSerialCom::PcSerialCom()
    : mUartUsb(USBTX, USBRX, BAUD_RATE)
{
}

} // namespace Drivers

