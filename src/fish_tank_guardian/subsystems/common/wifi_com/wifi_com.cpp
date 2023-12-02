/*!****************************************************************************
 * @file wifi_com.cpp
 * @brief TODO
 * @author Quattrone Martin
 * @date Oct 2023
 *******************************************************************************/

//=====[Libraries]=============================================================

#include "wifi_com.h"

#include "arm_book_lib.h"
#include "mbed.h"
#include "commands.h"
#include <string>

namespace Drivers {

//=====[Declaration and initialization of private global variables]============

WiFiCom* WiFiCom::mInstance              = nullptr;
static const char sResponseOk[]          = "OK";
static const char sResponseCwjapOk[]     = "+CWJAP:";
static const char sResponseCwjap1[]      = "WIFI CONNECTED";
static const char sResponseCwjap2[]      = "WIFI GOT IP";
static const char sResponseCifsr[]       = "+CIFSR:STAIP,\"";
static const char sResponseStatus3[]     = "STATUS:3";
static const char sResponseCipstatus[]   = "+CIPSTATUS:";
static const char sResponseSendOk[]      = "SEND OK";
static const char sResponseCipclose[]    = "CLOSED";

//=====[Implementations of public functions]===================================

//----static-------------------------------------------------------------------
void WiFiCom::Init()
{
    if (mInstance == nullptr)
    {
        mInstance = new WiFiCom(WIFI_PIN_TX, WIFI_PIN_RX, WIFI_BAUD_RATE);
    }

    // set init state
    mInstance->mState = WIFI_STATE::INIT;
    mInstance->mSsid = "";
    mInstance->mPassword = "";
    mInstance->mSerial.enable_output(false);
}

//----static-------------------------------------------------------------------
WiFiCom* WiFiCom::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
bool WiFiCom::IsBusy()
{
    return (mState != WIFI_STATE::IDLE);
}

//-----------------------------------------------------------------------------
void WiFiCom::Post(const std::string& server, const std::string& request)
{
    mState = WIFI_STATE::CMD_POST_SEND;
    mServer = server;
    mRequest = request;
    mResponse.clear();
}

//-----------------------------------------------------------------------------
void WiFiCom::Request(const std::string& url)
{
    mState = WIFI_STATE::CMD_GET_SEND;
    mServer = url;
    mRequest = "";
    mCommandGetResponse.clear();
}

//-----------------------------------------------------------------------------
bool WiFiCom::GetPostResponse(std::string* response)
{
    if (mState == WIFI_STATE::CMD_POST_RESPONSE_READY)
    {
        (*response) = mResponse;
        mResponse.clear();
        mIsResponseReady = false;

        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
bool WiFiCom::GetGetResponse(std::string* response)
{
    if (mState == WIFI_STATE::CMD_GET_RESPONSE_READY)
    {
        (*response) = mCommandGetResponse;
        mCommandGetResponse.clear();
        mIsGetResponseReady = false;

        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
void WiFiCom::Update()
{
    std::string esp32Command;
    static int sStartDelayTick;
    static int sDelayDuration;

    switch (mState) 
    {
        case WIFI_STATE::INIT:
        {
            mState = WIFI_STATE::CMD_STATUS_SEND;
            mWiFiComDelay.Start(DELAY_3_SECONDS);
        }
        break;

        case WIFI_STATE::CMD_STATUS_SEND:
        {
            if (mWiFiComDelay.HasFinished())
            {
                // sending status command to check if it's already connected to wifi
                mResponse.clear();
                esp32Command = COMMAND_STATUS_STR;
                esp32Command += STOP_CHAR;
                _SendCommand(esp32Command.c_str());
                mState = WIFI_STATE::CMD_STATUS_WAIT_RESPONSE;
                DEBUG_PRINT("WiFiCom - Checking WiFi status\r\n");
                mWiFiComDelay.Start(DELAY_3_SECONDS);
            }
        }
        break;

        case WIFI_STATE::CMD_STATUS_WAIT_RESPONSE:
        {
            const bool isResponseCompleted = _IsResponseCompleted(&mResponse);
            if ((mWiFiComDelay.HasFinished()) || (isResponseCompleted && (mResponse.compare(RESULT_NOT_CONNECTED) == 0)))
            {
                DEBUG_PRINT("CMD_STATUS_WAIT_RESPONSE - response = %s\r\n", mResponse.c_str());

                mState = WIFI_STATE::CMD_CONNECT_SEND;
                DEBUG_PRINT("WiFiCom - WiFi is not connected. Trying to connect...\r\n");
            }
            else if (isResponseCompleted && (mResponse.compare(RESULT_CONNECTED) == 0))
            {
                mState = WIFI_STATE::IDLE;
                DEBUG_PRINT("WiFiCom - WiFi is already connected.\r\n");
            }
        }
        break;

        case WIFI_STATE::CMD_CONNECT_SEND:
        {
            if (mWiFiComDelay.HasFinished())
            {
                // sending connect command with wifi ssid and password
                mResponse.clear();
                esp32Command = COMMAND_CONNECT_STR;
                esp32Command += PARAM_SEPARATOR_CHAR;
                esp32Command += mSsid;
                esp32Command += PARAM_SEPARATOR_CHAR;
                esp32Command += mPassword;
                esp32Command += STOP_CHAR;
                _SendCommand(esp32Command.c_str());
                mState = WIFI_STATE::CMD_CONNECT_WAIT_RESPONSE;
                DEBUG_PRINT("WiFiCom - Sending request to connect to WiFi Network [%s]...\r\n", mSsid.c_str());
                mWiFiComDelay.Start(DELAY_10_SECONDS);
            }
        }
        break;

        case WIFI_STATE::CMD_CONNECT_WAIT_RESPONSE:
        {
            const bool isResponseCompleted = _IsResponseCompleted(&mResponse);
            if ((mWiFiComDelay.HasFinished()) || (isResponseCompleted && (mResponse.compare(RESULT_ERROR) == 0)))
            {
                DEBUG_PRINT("WiFiCom - [ERROR] WiFi Not Connected. Response = [%s]\r\n", mResponse.c_str());
                mState = WIFI_STATE::CMD_ACCESSPOINT_SEND;
                mWiFiComDelay.Start(DELAY_2_SECONDS);
            }
            else if (isResponseCompleted && (mResponse.compare(RESULT_OK) == 0))
            {
                mState = WIFI_STATE::IDLE;
                DEBUG_PRINT("WiFiCom - [OK] Success. WiFi connected.\r\n");
            }
        }
        break;

        case WIFI_STATE::CMD_ACCESSPOINT_SEND:
        {
            if (mWiFiComDelay.HasFinished())
            {
                mResponse.clear();
                esp32Command = COMMAND_ACCESSPOINT_STR;
                esp32Command += STOP_CHAR;
                _SendCommand(esp32Command.c_str());
                mState = WIFI_STATE::CMD_ACCESSPOINT_WAIT_RESPONSE;
                DEBUG_PRINT("WiFiCom - Setting AccessPoint\r\n");
            }
        }
        break;

        case WIFI_STATE::CMD_ACCESSPOINT_WAIT_RESPONSE:
        {
            const bool isResponseCompleted = _IsResponseCompleted(&mResponse);
            if (isResponseCompleted && (mResponse.compare(RESULT_ERROR) == 0))
            {
                mState = WIFI_STATE::CMD_ACCESSPOINT_SEND;
                DEBUG_PRINT("WiFiCom - [ERROR] Access Point returned error\r\n");
            }
            else if (isResponseCompleted)
            {
                int paramIndex = mResponse.find(PARAM_SEPARATOR_CHAR);
                if (paramIndex != std::string::npos)
                {
                    DEBUG_PRINT("WiFiCom - [OK] Success. Response to [%s] obtained\r\n", COMMAND_ACCESSPOINT_STR);
                    mSsid       = mResponse.substr(0, paramIndex);
                    mPassword   = mResponse.substr(paramIndex + 1, mResponse.length());
                    mWiFiComDelay.Start(DELAY_2_SECONDS);
                    mState = WIFI_STATE::CMD_CONNECT_SEND;
                }
                else
                {
                    DEBUG_PRINT("WiFiCom - [ERROR] Invalid Response to [%s] obtained\r\n", COMMAND_ACCESSPOINT_STR);
                    mState = WIFI_STATE::CMD_ACCESSPOINT_SEND;
                }
            }
        }
        break;

        case WIFI_STATE::CMD_GET_SEND:
        {
            mCommandGetResponse.clear();
            esp32Command = COMMAND_GET_STR;
            esp32Command += PARAM_SEPARATOR_CHAR;
            esp32Command += mServer;
            esp32Command += STOP_CHAR;
            _SendCommand(esp32Command.c_str());
            mState = WIFI_STATE::CMD_GET_WAIT_RESPONSE;
            // DEBUG_PRINT("WiFiCom - [%s] Sent request\r\n", COMMAND_GET_STR);
            mWiFiComDelay.Start(DELAY_3_SECONDS);
        }
        break;

        case WIFI_STATE::CMD_GET_WAIT_RESPONSE:
        {
            const bool isResponseCompleted = _IsResponseCompleted(&mCommandGetResponse);
            if ((mWiFiComDelay.HasFinished()) || (isResponseCompleted && (mCommandGetResponse.compare(RESULT_ERROR) == 0)))
            {
                mState = WIFI_STATE::ERROR;
                // DEBUG_PRINT("WiFiCom - [ERROR] [%s]\r\n", COMMAND_GET_STR);
            }
            else if (isResponseCompleted)
            {
                mState = WIFI_STATE::CMD_GET_RESPONSE_READY;
                mIsGetResponseReady = true;
                // DEBUG_PRINT("WiFiCom - [OK] [%s]\r\n", COMMAND_GET_STR);
                mWiFiComDelay.Start(DELAY_10_SECONDS);                  // a timeout is set to avoid a hang on
            }
        }
        break;

        case WIFI_STATE::CMD_GET_RESPONSE_READY:
        {
            // Wait until the response is grabbed and clear or wait or the time out
            if (!mIsGetResponseReady)
            {
                mState = WIFI_STATE::IDLE;
            }
            else if (mWiFiComDelay.HasFinished()) 
            {
                mState = WIFI_STATE::ERROR;
                mCommandGetResponse.clear();
                // DEBUG_PRINT("WiFiCom - [ERROR] [%s] Response deleted for timeout\r\n", COMMAND_GET_STR);
            }
        }
        break;

        case WIFI_STATE::CMD_POST_SEND:
        {
            // sending get command with wifi ssid and password
            mResponse.clear();  
            esp32Command = COMMAND_POST_STR;
            esp32Command += PARAM_SEPARATOR_CHAR;
            esp32Command += mServer;
            esp32Command += PARAM_SEPARATOR_CHAR;
            esp32Command += mRequest;
            esp32Command += STOP_CHAR;
            _SendCommand(esp32Command.c_str());
            mState = WIFI_STATE::CMD_POST_WAIT_RESPONSE;
            // DEBUG_PRINT("WiFiCom - [%s] Sent request\r\n", COMMAND_POST_STR);
            mWiFiComDelay.Start(DELAY_3_SECONDS);
        }
        break;

        case WIFI_STATE::CMD_POST_WAIT_RESPONSE:
        {
            const bool isResponseCompleted = _IsResponseCompleted(&mResponse);
            if ((mWiFiComDelay.HasFinished()) || (isResponseCompleted && (mResponse.compare(RESULT_ERROR) == 0)))
            {
                mState = WIFI_STATE::CMD_POST_RESPONSE_READY;
                mResponse = RESULT_ERROR;
                // DEBUG_PRINT("WiFiCom - [ERROR] [%s]\r\n", COMMAND_POST_STR);
            }
            else if (isResponseCompleted)
            {
                mState = WIFI_STATE::CMD_POST_RESPONSE_READY;
                mIsResponseReady = true;
                // DEBUG_PRINT("WiFiCom - [OK] [%s]\r\n", COMMAND_POST_STR);
                mWiFiComDelay.Start(DELAY_10_SECONDS);                  // a timeout is set to avoid a hang on
            }
        }
        break;

        case WIFI_STATE::CMD_POST_RESPONSE_READY:
        {
            // Wait until the response is grabbed and clear or wait or the time out
            if (!mIsResponseReady)
            {
                mState = WIFI_STATE::IDLE;
            }
            else if (mWiFiComDelay.HasFinished()) 
            {
                mState = WIFI_STATE::ERROR;
                mResponse.clear();
                // DEBUG_PRINT("WiFiCom - [ERROR] [%s] Response deleted for timeout\r\n", COMMAND_POST_STR);
            }
        }
        break;

        case WIFI_STATE::ERROR:
        {
            mState = WIFI_STATE::IDLE;
        }
        break;
        case WIFI_STATE::IDLE:
        break;
    }
}


//=====[Implementations of private functions]==================================

//-----------------------------------------------------------------------------
WiFiCom::WiFiCom(PinName txPin, PinName rxPin, const int baudRate)
    : mSerial(txPin, rxPin, baudRate)
    , mWiFiComDelay(0)
{
}
//-----------------------------------------------------------------------------
void WiFiCom::_SendCommand(const char* command)
{
    mSerial.enable_output(true);
    mSerial.write(command, strlen(command));
    mSerial.enable_output(false);
}

//-----------------------------------------------------------------------------
bool WiFiCom::_IsResponseCompleted(std::string* response)
{
    char receivedChar;

    if (_ReadCom(&receivedChar))
    {
        if (receivedChar == STOP_CHAR) 
        {
            return true;
        } 
        else 
        {
            (*response) += receivedChar;
        }
    }

    return false;
}

//-----------------------------------------------------------------------------
bool WiFiCom::_ReadCom(char* receivedChar)
{
    char receivedCharLocal = '\0';

    if(mSerial.readable()) 
    {
        mSerial.read(&receivedCharLocal, 1);
        (*receivedChar) = receivedCharLocal;

        return true;
    }

    return false;
}

} // namespace Drivers