/*!****************************************************************************
 * @file wifi.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Nov 2023
 *******************************************************************************/

#ifndef WIFI_H
#define WIFI_H

#include "mbed.h"
#include <string>

#define WIFI_PIN_TX A1
#define WIFI_PIN_RX A2
#define WIFI_BAUD_RATE 115200

namespace Drivers { 

    class Wifi 
    {
        public:

            //! Initialize the module
            static void Init();

            //! Returns the module object
            static Wifi* GetInstance();

            //!
            void Update();


        private:

            static Wifi* mInstance;
            UnbufferedSerial mSerial;

            Wifi(PinName txPin, PinName rxPin, const int baudRate);
            ~Wifi() = default;
            Wifi(const Wifi&) = delete;
            Wifi& operator=(const Wifi&) = delete;

            void SerialWrite(std::string str);



    };

} // namespace Drivers

#endif // WIFI_H