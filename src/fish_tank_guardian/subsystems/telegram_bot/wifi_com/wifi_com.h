/*!****************************************************************************
 * @file wifi_com.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Nov 2023
 *******************************************************************************/

#ifndef WIFI_COM
#define WIFI_COM

#include <string>
#include "delay.h"
#include "mbed.h"

#define WIFI_PIN_TX             PE_8
#define WIFI_PIN_RX             PE_7
#define WIFI_BAUD_RATE          115200
#define DELAY_10_SECONDS        10000
#define DELAY_5_SECONDS         5000
#define DELAY_2_SECONDS         2000

namespace Drivers { 

    class WiFiCom 
    {
        public:

            //! Initialize the module
            static void Init();

            //! Returns the module object
            static WiFiCom* GetInstance();

            //! Udpate
            void Update();

        private:

            enum class WIFI_STATE
            {
                INIT,
                CHECK_STATUS,
                WAIT_CHECK_STATUS_RESPONSE,
                CONNECT_REQUEST,
                WAIT_CONNECT_RESPONSE,
                IDLE,
                ERROR
            };

            WiFiCom(PinName txPin, PinName rxPin, const int baudRate);
            ~WiFiCom() = default;
            WiFiCom(const WiFiCom&) = delete;
            WiFiCom& operator=(const WiFiCom&) = delete;

            //!
            void _Write(const char* format, ...);

            //!
            bool _ReadCom(char* receivedChar);

            //!
            bool _IsExpectedResponse();

            static WiFiCom*     mInstance;
            BufferedSerial      mSerial;
            WIFI_STATE          mState;
            Util::Delay         mWiFiComDelay;
            std::string         mExpectedResponse;
            std::string         mApSsid;
            std::string         mApPassword;
    };

} // namespace Drivers

#endif // WIFI_COM