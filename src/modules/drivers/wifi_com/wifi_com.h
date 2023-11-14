/*!****************************************************************************
 * @file wifi_com.h
 * @brief TODO
 * @author Quattrone Martin
 * @date Nov 2023
 *******************************************************************************/

#ifndef WIFI_COM
#define WIFI_COM

#include "mbed.h"
#include <string>
#include "delay.h"

#define WIFI_PIN_TX             PE_8
#define WIFI_PIN_RX             PE_7
#define WIFI_BAUD_RATE          9600
#define DELAY_10_SECONDS        10000
#define DELAY_5_SECONDS         5000

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
                SEND_AT,
                WAIT_AT,
                SEND_CWMODE,
                WAIT_CWMODE,
                SEND_CWJAP_IS_SET,
                WAIT_CWJAP_IS_SET,
                SEND_CWJAP_SET,
                WAIT_CWJAP_SET_1,
                WAIT_CWJAP_SET_2,
                SEND_CIFSR,
                WAIT_CIFSR,
                LOAD_IP,
                SEND_CIPMUX,
                WAIT_CIPMUX,
                SEND_CIPSERVER,
                WAIT_CIPSERVER,
                SEND_CIPSTATUS,
                WAIT_CIPSTATUS_STATUS_3,
                WAIT_CIPSTATUS,
                WAIT_GET_ID,
                WAIT_CIPSTATUS_OK,
                SEND_CIPSEND,
                WAIT_CIPSEND,
                SEND_HTML,
                WAIT_HTML,
                SEND_CIPCLOSE,
                WAIT_CIPCLOSE,
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
            bool ReadCom(char* receivedChar);

            //!
            bool IsExpectedResponse();

            static WiFiCom*     mInstance;
            UnbufferedSerial    mSerial;
            WIFI_STATE          mState;
            Util::Delay         mWiFiComDelay;
            std::string         mExpectedResponse;
            char*               mApSsid;
            char*               mApPassword;
    };

} // namespace Drivers

#endif // WIFI_COM