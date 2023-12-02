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
            void Request(const std::string& url);

            //!
            bool GetPostResponse(std::string* response);

            //!
            bool GetGetResponse(std::string* response);

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
                CMD_ACCESSPOINT_SEND,
                CMD_ACCESSPOINT_WAIT_RESPONSE,
                CMD_GET_SEND,
                CMD_GET_WAIT_RESPONSE,
                CMD_GET_RESPONSE_READY,
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
            void _SendCommand(const char* command);

            //!
            bool _IsResponseCompleted(std::string* response);

            //!
            bool _ReadCom(char* receivedChar);

            static WiFiCom*     mInstance;
            UnbufferedSerial    mSerial;
            WIFI_STATE          mState;
            Util::Delay         mWiFiComDelay;
            std::string         mSsid;
            std::string         mPassword;
            std::string         mResponse;
            std::string         mCommandGetResponse;
            std::string         mServer;
            std::string         mRequest;
            bool                mIsResponseReady;
            bool                mIsGetResponseReady;
    };

} // namespace Drivers

#endif // WIFI_COM