/*!****************************************************************************
 * @file wifi_com.h
 * @brief   Declaration of WiFiCom class
 * @author Quattrone Martin
 * @date Nov 2023
 *******************************************************************************/

#ifndef WIFI_COM
#define WIFI_COM

#include <string>
#include "delay.h"
#include "mbed.h"
#include "real_time_clock.h"

#define WIFI_PIN_TX                     PE_8
#define WIFI_PIN_RX                     PE_7
#define WIFI_BAUD_RATE                  115200
#define WIFI_SSID_EEPROM_START          8 * RTC_EEPROM_PAGE_SIZE
#define WIFI_PASS_EEPROM_START          9 * RTC_EEPROM_PAGE_SIZE

namespace Drivers { 

    class WiFiCom 
    {
        public:

            /**
            * @brief Initializes the WiFi module.
            */
            static void Init();

            /**
            * @brief Gets the unique instance of the WiFi module.
            * @return Pointer to the WiFiCom instance.
            */
            static WiFiCom* GetInstance();

            /**
            * @brief Updates the WiFi module state.
            */
            void Update();

            /**
            * @brief Checks if the WiFi module is busy.
            * @return True if the WiFi module is busy, false otherwise.
            */
            bool IsBusy();

            /**
            * @brief Performs a POST request to the specified server.
            * @param server The server to send the request to.
            * @param request The request data.
            */
            void Post(const std::string& server, const std::string& request);

            /**
            * @brief Performs a GET request to the specified URL.
            * @param url The URL to send the request to.
            */
            void Request(const std::string& url);

            /**
            * @brief Gets the response from the last POST request.
            * @param response Pointer to store the response.
            * @return True if there is a response, false otherwise.
            */
            bool GetPostResponse(std::string* response);

            /**
            * @brief Gets the response from the last GET request.
            * @param response Pointer to store the response.
            * @return True if there is a response, false otherwise.
            */
            bool GetGetResponse(std::string* response);

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

            /**
            * @brief Sends a command to the ESP32 module.
            * @param command The command to send.
            */
            void _SendCommand(const char* command);

            /**
            * @brief Checks if the response is completed.
            * @param response Pointer to store the response.
            * @return True if the response is completed, false otherwise.
            */
            bool _IsResponseCompleted(std::string* response);

            /**
            * @brief Reads a character from the communication interface.
            * @param receivedChar Pointer to store the received character.
            * @return True if a character is successfully read, false otherwise.
            */
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