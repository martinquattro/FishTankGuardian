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

    mInstance->mState = WIFI_STATE::INIT;
    mInstance->mApSsid = "Cuchitril 2.4GHz";
    mInstance->mApPassword = "Defensa5232022";
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
}

//-----------------------------------------------------------------------------
void WiFiCom::Request(const std::string& url)
{
    mState = WIFI_STATE::CMD_GET_SEND;
    mServer = url;
}

//-----------------------------------------------------------------------------
std::string WiFiCom::GetResponse()
{
    std::string responseAux = mResponse;
    mResponse.clear();
    mIsPostResponseReady = false;
    return responseAux;
}

//-----------------------------------------------------------------------------
bool WiFiCom::IsPostResponseReady()
{
    return (mState == WIFI_STATE::CMD_POST_RESPONSE_READY);
}

//-----------------------------------------------------------------------------
void WiFiCom::Update()
{
    std::string esp32Command;

    switch (mState) 
    {
        case WIFI_STATE::INIT:
        {
            mWiFiComDelay.Start(DELAY_2_SECONDS);
            mState = WIFI_STATE::CMD_STATUS_SEND;
        }
        break;

        case WIFI_STATE::CMD_STATUS_SEND:
        {
            if (mWiFiComDelay.HasFinished())
            {
                // sending status command to check if it's already connected to wifi
                mResponse.clear();
                mWiFiComDelay.Start(DELAY_2_SECONDS);
                esp32Command = COMMAND_STATUS_STR;
                _SendCommand(esp32Command.c_str());
                mState = WIFI_STATE::CMD_STATUS_WAIT_RESPONSE;
                DEBUG_PRINT("WiFiCom::Update() - Sending [%s]\r\n", COMMAND_STATUS_STR);
            }
        }
        break;

        case WIFI_STATE::CMD_STATUS_WAIT_RESPONSE:
        {
            const bool isResponseCompleted = _IsResponseCompleted();
            if ((mWiFiComDelay.HasFinished()) || (isResponseCompleted && (mResponse.compare(RESULT_NOT_CONNECTED) == 0)))
            {
                mState = WIFI_STATE::CMD_CONNECT_SEND;
                DEBUG_PRINT("WiFiCom::Update() - ERROR WiFi is not connected. Response = [%s] \r\n", mResponse.c_str());
            }
            else if (isResponseCompleted && (mResponse.compare(RESULT_CONNECTED) == 0))
            {
                mState = WIFI_STATE::IDLE;
                DEBUG_PRINT("WiFiCom::Update() - WiFi is already connected.\r\n");
            }
        }
        break;

        case WIFI_STATE::CMD_CONNECT_SEND:
        {
            if (mWiFiComDelay.HasFinished())
            {
                // sending connect command with wifi ssid and password
                mResponse.clear();
                mWiFiComDelay.Start(DELAY_5_SECONDS);
                esp32Command = COMMAND_CONNECT_STR;
                esp32Command += PARAM_SEPARATOR_CHAR;
                esp32Command += mApSsid;
                esp32Command += PARAM_SEPARATOR_CHAR;
                esp32Command += mApPassword;
                _SendCommand(esp32Command.c_str());
                mState = WIFI_STATE::CMD_CONNECT_WAIT_RESPONSE;
                DEBUG_PRINT("WiFiCom::Update() - Sending [%s]\r\n", esp32Command.c_str());
            }
        }
        break;

        case WIFI_STATE::CMD_CONNECT_WAIT_RESPONSE:
        {
            const bool isResponseCompleted = _IsResponseCompleted();
            if ((mWiFiComDelay.HasFinished()) || (isResponseCompleted && (mResponse.compare(RESULT_ERROR) == 0)))
            {
                mState = WIFI_STATE::ERROR;
                DEBUG_PRINT("WiFiCom::Update() - ERROR [%s] command not responded correctly. Wifi Not Connected. Response = [%s]\r\n", COMMAND_CONNECT_STR, mResponse.c_str());
            }
            else if (isResponseCompleted && (mResponse.compare(RESULT_OK) == 0))
            {
                mState = WIFI_STATE::IDLE;
                DEBUG_PRINT("WiFiCom::Update() - Success. WiFi connected.\r\n");
            }
        }
        break;

        case WIFI_STATE::CMD_GET_SEND:
        {
            // sending get command with wifi ssid and password
            mResponse.clear();
            mWiFiComDelay.Start(DELAY_10_SECONDS);
            esp32Command = COMMAND_GET_STR;
            esp32Command += PARAM_SEPARATOR_CHAR;
            esp32Command += "https://www.timeapi.io/api/Time/current/zone?timeZone=America/Buenos_Aires";
            _SendCommand(esp32Command.c_str());
            mState = WIFI_STATE::CMD_GET_WAIT_RESPONSE;
            DEBUG_PRINT("WiFiCom::Update() - Sending [%s]\r\n", esp32Command.c_str());
        }
        break;

        case WIFI_STATE::CMD_GET_WAIT_RESPONSE:
        {
            const bool isResponseCompleted = _IsResponseCompleted();
            if ((mWiFiComDelay.HasFinished()) || (isResponseCompleted && (mResponse.compare(RESULT_ERROR) == 0)))
            {
                mState = WIFI_STATE::ERROR;
                DEBUG_PRINT("WiFiCom::Update() - [%s] command not responded correctly\r\n", COMMAND_GET_STR);
            }
            else if (isResponseCompleted)
            {
                mState = WIFI_STATE::IDLE;
                DEBUG_PRINT("WiFiCom::Update() - Response obtained correctly.\r\n%s\r\n", mResponse.c_str());
            }
        }
        break;

        case WIFI_STATE::CMD_POST_SEND:
        {
            // sending get command with wifi ssid and password
            mResponse.clear();  
            mWiFiComDelay.Start(DELAY_10_SECONDS);
            esp32Command = COMMAND_POST_STR;
            esp32Command += PARAM_SEPARATOR_CHAR;
            esp32Command += mServer;
            esp32Command += PARAM_SEPARATOR_CHAR;
            esp32Command += mRequest;
            _SendCommand(esp32Command.c_str());
            mState = WIFI_STATE::CMD_POST_WAIT_RESPONSE;
            // DEBUG_PRINT("WiFiCom::Update() - Sending [%s]\r\n", esp32Command.c_str());
        }
        break;

        case WIFI_STATE::CMD_POST_WAIT_RESPONSE:
        {
            const bool isResponseCompleted = _IsResponseCompleted();
            if ((mWiFiComDelay.HasFinished()) || (isResponseCompleted && (mResponse.compare(RESULT_ERROR) == 0)))
            {
                mState = WIFI_STATE::ERROR;
                DEBUG_PRINT("WiFiCom::Update() - ERROR [%s] command not responded correctly\r\n", COMMAND_POST_STR);
            }
            else if (isResponseCompleted)
            {
                mState = WIFI_STATE::CMD_POST_RESPONSE_READY;
                mWiFiComDelay.Start(DELAY_10_SECONDS);                  // a timeout is set to avoid a hang on
                mIsPostResponseReady = true;
                // DEBUG_PRINT("WiFiCom::Update() - [%s] responded correctly.\r\n", COMMAND_POST_STR);
            }
        }
        break;

        case WIFI_STATE::CMD_POST_RESPONSE_READY:
        {
            // Wait until the response is grabbed and clear or wait or the time out
            if (!mIsPostResponseReady)
            {
                mState = WIFI_STATE::IDLE;
                // DEBUG_PRINT("WiFiCom::Update() - Response grabbed.\r\n");
            }
            else if (mWiFiComDelay.HasFinished()) 
            {
                mState = WIFI_STATE::ERROR;
                mResponse.clear();
                DEBUG_PRINT("WiFiCom::Update() - Response was never grabbed and it got lost\r\n");
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
    std::string message = command + STOP_CHAR;
    mSerial.write(message.c_str(), message.size());
}

//-----------------------------------------------------------------------------
bool WiFiCom::_IsResponseCompleted()
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
            mResponse += receivedChar;
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