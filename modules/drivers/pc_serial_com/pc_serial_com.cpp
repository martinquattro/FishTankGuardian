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
    // Inicializar la lista de argumentos variables
    va_list args;
    va_start(args, format);

    // Calcular la longitud de la cadena formateada
    int length = vsnprintf(nullptr, 0, format, args);

    // Reservar memoria para la cadena formateada
    char* buffer = new char[length + 1];

    // Formatear la cadena y almacenarla en el búfer
    vsnprintf(buffer, length + 1, format, args);

    // Limpiar la lista de argumentos variables
    va_end(args);

    // Escribir la cadena en el puerto serial
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

