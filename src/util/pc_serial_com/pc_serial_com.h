/*!****************************************************************************
 * @file    pc_serial_com.h
 * @brief   Declaration of PC serial communication module methods and definitions
 * @author  Quattrone Martin
 * @date    Oct 2023
 *******************************************************************************/

#ifndef PC_SERIAL_COM_H
#define PC_SERIAL_COM_H

#include "mbed.h"

#define BAUD_RATE 115200

namespace Util { 

    class PcSerialCom 
    {
        public:

            /**
            * @brief Initialize the PC serial communication module.
            */
            static void Init();

        private:

            /**
            * @brief Constructor for PcSerialCom class.
            * @param txPin The transmission pin.
            * @param rxPin The reception pin.
            * @param baudRate The baud rate for serial communication.
            */
            PcSerialCom(PinName txPin, PinName rxPin, const int baudRate);
            
            ~PcSerialCom() = default;
            PcSerialCom(const PcSerialCom&) = delete;
            PcSerialCom& operator=(const PcSerialCom&) = delete;

            static PcSerialCom* mInstance;
            UnbufferedSerial    mUartUsb;
    };

} // namespace Util

#endif // PC_SERIAL_COM_H