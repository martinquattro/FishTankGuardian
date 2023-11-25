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
void WiFiCom::Update()
{
    // char command[100];
    // bool waitingResponse;
  
    // char receivedCharLocal = '\0';
    // while (ReadCom(&receivedCharLocal))
    // {
    //     DEBUG_PRINT("%c\r\n", receivedCharLocal);

    //     if (receivedCharLocal == '|')
    //     {
    //         DEBUG_PRINT("\r\n");
    //     }
    // }

    std::string esp32Command;

    switch (mState) 
    {
        case WIFI_STATE::INIT:
        {
            mWiFiComDelay.Start(DELAY_2_SECONDS);
            mState = WIFI_STATE::CHECK_STATUS;
        }
        break;

        case WIFI_STATE::CHECK_STATUS:
        {
            if (mWiFiComDelay.HasFinished())
            {
                // sending status command to check if it's already connected to wifi
                esp32Command = COMMAND_STATUS_STR;
                _Write(esp32Command.c_str());
                mExpectedResponse = RESULT_CONNECTED;
                mWiFiComDelay.Start(DELAY_2_SECONDS);
                mState = WIFI_STATE::WAIT_CHECK_STATUS_RESPONSE;
                DEBUG_PRINT("WiFiCom::Update() - [%s] command Sent\r\n", esp32Command.c_str());
            }
        }
        break;

        case WIFI_STATE::WAIT_CHECK_STATUS_RESPONSE:
        {
            if (_IsExpectedResponse()) 
            {
                mState = WIFI_STATE::IDLE;
                DEBUG_PRINT("WiFiCom::Update() - [%s] command responded wifi is already connected\r\n", COMMAND_STATUS_STR);
            }
            else if (mWiFiComDelay.HasFinished()) 
            {
                mState = WIFI_STATE::CONNECT_REQUEST;
                DEBUG_PRINT("WiFiCom::Update() - [%s] command not responded correctly. Trying to connect now.\r\n", COMMAND_STATUS_STR);
            }
        }
        break;

        case WIFI_STATE::CONNECT_REQUEST:
        {
            if (mWiFiComDelay.HasFinished())
            {
                // sending connect command with wifi ssid and password
                esp32Command = COMMAND_CONNECT_STR;
                esp32Command += PARAM_SEPARATOR;
                esp32Command += mApSsid;
                esp32Command += PARAM_SEPARATOR;
                esp32Command += mApPassword;
                _Write(esp32Command.c_str());
                mExpectedResponse = RESULT_OK;
                mWiFiComDelay.Start(DELAY_5_SECONDS);
                mState = WIFI_STATE::WAIT_CONNECT_RESPONSE;
                DEBUG_PRINT("WiFiCom::Update() - [%s] command Sent\r\n", esp32Command.c_str());
            }
        }
        break;

        case WIFI_STATE::WAIT_CONNECT_RESPONSE:
        {
            if (_IsExpectedResponse()) 
            {
                mState = WIFI_STATE::IDLE;
                DEBUG_PRINT("WiFiCom::Update() - [%s] command responded correctly. Wifi connected.\r\n", COMMAND_CONNECT_STR);
            }
            else if (mWiFiComDelay.HasFinished()) 
            {
                mState = WIFI_STATE::ERROR;
                DEBUG_PRINT("WiFiCom::Update() - [%s] command not responded correctly\r\n", COMMAND_CONNECT_STR);
            }
        }
        break;

        case WIFI_STATE::IDLE:
        case WIFI_STATE::ERROR:
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
void WiFiCom::_Write(const char* format, ...)
{
    // Initialize the variable arguments list
    va_list args;
    va_start(args, format);

    // Calculate the length of the formatted string
    int length = vsnprintf(nullptr, 0, format, args);

    // Allocate memory for the formatted string
    char* buffer = new char[length + 1];

    // Format the string and store it in the buffer
    vsnprintf(buffer, length + 1, format, args);

    // Clean up the variable arguments list
    va_end(args);

    // Write the string to the serial port
    const int bytesWritten = mSerial.write(buffer, strlen(buffer));
    
    delete[] buffer;
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

//-----------------------------------------------------------------------------
bool WiFiCom::_IsExpectedResponse()
{
    static int responseStringPositionIndex = 0;
    char charReceived;
    bool moduleResponse = false;

    if(_ReadCom(&charReceived))
    {
        if (charReceived == mExpectedResponse[responseStringPositionIndex]) 
        {
            responseStringPositionIndex++;
            if (mExpectedResponse[responseStringPositionIndex] == '\0') 
            {
                responseStringPositionIndex = 0;
                moduleResponse = true;
            }
        } 
        else 
        {
            responseStringPositionIndex = 0;
        }
    }

    return moduleResponse;
}

} // namespace Drivers