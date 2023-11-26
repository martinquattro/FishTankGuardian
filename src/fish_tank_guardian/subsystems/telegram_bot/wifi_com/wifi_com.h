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

            //! Returns the module object
            bool IsBusy();

            //!
            void Post(const std::string& server, const std::string& request);

            //!
            std::string GetResponse();

            //!
            bool IsPostResponseReady();

            //! Udpate
            void Update();

        private:

            enum class WIFI_STATE
            {
                INIT,
                CMD_STATUS_SEND,
                CMD_STATUS_WAIT_RESPONSE,
                CMD_CONNECT_SEND,
                CMD_CONNECT_WAIT_RESPONSE,
                CMD_GET_SEND,
                CMD_GET_WAIT_RESPONSE,
                CMD_POST_SEND,
                CMD_POST_WAIT_RESPONSE,
                CMD_POST_RESPONSE_READY,
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
            bool _IsResponseCompleted();

            //!
            bool _ReadCom(char* receivedChar);

            static WiFiCom*     mInstance;
            BufferedSerial      mSerial;
            WIFI_STATE          mState;
            Util::Delay         mWiFiComDelay;
            std::string         mApSsid;
            std::string         mApPassword;
            std::string         mResponse;
            std::string         mServer;
            std::string         mRequest;
            bool                mIsPostResponseReady;
    };

} // namespace Drivers

#endif // WIFI_COM