/*!****************************************************************************
 * @file pc_serial_com.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

#ifndef PC_SERIAL_COM_H
#define PC_SERIAL_COM_H

#include "mbed.h"

#define BAUD_RATE 115200

namespace Util { 

    class PcSerialCom 
    {
        public:

            //! Initialize the sensor
            static void Init();

        private:

            PcSerialCom(PinName txPin, PinName rxPin, const int baudRate);
            ~PcSerialCom() = default;
            PcSerialCom(const PcSerialCom&) = delete;
            PcSerialCom& operator=(const PcSerialCom&) = delete;

            static PcSerialCom* mInstance;
            UnbufferedSerial mUartUsb;

    };

} // namespace Util

#endif // PC_SERIAL_COM_H   