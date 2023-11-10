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

namespace Drivers { 

    class PcSerialCom 
    {
        public:

            //! Initialize the sensor
            static void Init();

            //! Returns the sensor object
            static PcSerialCom* GetInstance();

            //! Write string through usb uart
            void Write(const char* format, ...);

        private:

            PcSerialCom();
            ~PcSerialCom() = default;
            PcSerialCom(const PcSerialCom&) = delete;
            PcSerialCom& operator=(const PcSerialCom&) = delete;

            static PcSerialCom* mInstance;
            UnbufferedSerial mUartUsb;

    };

} // namespace Drivers

#endif // PC_SERIAL_COM_H