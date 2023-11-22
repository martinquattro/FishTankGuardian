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
}

//----static-------------------------------------------------------------------
WiFiCom* WiFiCom::GetInstance()
{
    return mInstance;
}

//-----------------------------------------------------------------------------
void WiFiCom::Update()
{
    char command[100];

    switch (mState) 
    {
        case WIFI_STATE::INIT:
        {
            mWiFiComDelay.Start(DELAY_10_SECONDS);
            mState = WIFI_STATE::SEND_AT;
        }
        break;
        case WIFI_STATE::SEND_AT:
        {
            if (mWiFiComDelay.HasFinished())
            {
                sprintf(command, "AT");
                _Write("%s\r\n", command);
                mExpectedResponse = sResponseOk;
                mWiFiComDelay.Start(DELAY_5_SECONDS);
                mState = WIFI_STATE::WAIT_AT;
                DEBUG_PRINT("WiFiCom::Update() - [%s] command Sent\r\n", command);
            }
        }
        break;
        case WIFI_STATE::WAIT_AT:
        {
            if (IsExpectedResponse()) 
            {
                mWiFiComDelay.Start(DELAY_5_SECONDS);
                mState = WIFI_STATE::SEND_CWMODE;
                DEBUG_PRINT("WiFiCom::Update() - AT command responded correctly\r\n");
            }
            else if (mWiFiComDelay.HasFinished()) 
            {
                mState = WIFI_STATE::ERROR;
                DEBUG_PRINT("WiFiCom::Update() - AT command not responded correctly\r\n");
            }
        }
        break;
        case WIFI_STATE::SEND_CWMODE:
        {
            if (mWiFiComDelay.HasFinished()) 
            {
                sprintf(command, "AT+CWMODE=1");
                _Write("%s\r\n", command);
                mExpectedResponse = sResponseOk;
                mWiFiComDelay.Start(DELAY_5_SECONDS);
                mState = WIFI_STATE::WAIT_CWMODE;
                DEBUG_PRINT("WiFiCom::Update() - [%s] command Sent\r\n", command);
            }
        }
        break;
        case WIFI_STATE::WAIT_CWMODE:
        {
            if (IsExpectedResponse()) 
            {
                mWiFiComDelay.Start(DELAY_5_SECONDS);
                mState = WIFI_STATE::SEND_CWJAP_IS_SET;
                DEBUG_PRINT("WiFiCom::Update() - AT+CWMODE=1 responded correctly\r\n");
            }

            if (mWiFiComDelay.HasFinished()) 
            {
                mState = WIFI_STATE::ERROR;
                DEBUG_PRINT("WiFiCom::Update() - AT+CWMODE=1 command not responded correctly\r\n");
            }
        }
        break;
        case WIFI_STATE::SEND_CWJAP_IS_SET:
        {
            if (mWiFiComDelay.HasFinished()) 
            {
                sprintf(command, "AT+CWJAP?");
                _Write("%s\r\n", command);
                mExpectedResponse = sResponseCwjapOk;
                mWiFiComDelay.Start(DELAY_5_SECONDS);
                mState = WIFI_STATE::WAIT_CWJAP_IS_SET;
                DEBUG_PRINT("WiFiCom::Update() - [%s] command sent\r\n", command);
            }
        }
        break;
        case WIFI_STATE::WAIT_CWJAP_IS_SET:
        {
            if (IsExpectedResponse())
            {
                mExpectedResponse = sResponseOk;
                mState = WIFI_STATE::SEND_CIFSR;
            }

            if (mWiFiComDelay.HasFinished()) 
            {
                mWiFiComDelay.Start(DELAY_5_SECONDS);
                mState = WIFI_STATE::SEND_CWJAP_SET;
            }
        }
        break;
        case WIFI_STATE::SEND_CWJAP_SET:
        {
            if (mWiFiComDelay.HasFinished()) 
            {
                sprintf(command, "AT+CWJAP=\"%s\",\"%s\"", mApSsid.c_str(), mApPassword.c_str());
                _Write("%s\r\n", command);
                mExpectedResponse = sResponseCwjap1;
                mWiFiComDelay.Start(DELAY_10_SECONDS);
                mState = WIFI_STATE::WAIT_CWJAP_SET_1;
                DEBUG_PRINT("WiFiCom::Update() - [%s] command sent\r\n", command);
            }
        }
        break;
        case WIFI_STATE::WAIT_CWJAP_SET_1:
        {
            if (IsExpectedResponse()) 
            {
                mExpectedResponse = sResponseCwjap2;
                mState = WIFI_STATE::WAIT_CWJAP_SET_2;
            }

            if (mWiFiComDelay.HasFinished()) 
            {
                DEBUG_PRINT("WiFiCom::Update() - Error in state = WIFI_STATE::WAIT_CWJAP_SET_1. Check Wi-Fi AP credentials\r\n");
                mState = WIFI_STATE::ERROR;
            }
            break;
        }
        case WIFI_STATE::WAIT_CWJAP_SET_2:
        {
            if (IsExpectedResponse())
            {
                mState = WIFI_STATE::SEND_CIFSR;
            }

            if (mWiFiComDelay.HasFinished()) 
            {
                // DEBUG_PRINT("WiFiCom::Update() - Error in state = WIFI_STATE::WAIT_CWJAP_SET_2. Check Wi-Fi AP credentials");
                mState = WIFI_STATE::ERROR;
            }
        }
        break;
        // case WIFI_STATE::SEND_CIFSR:
        // {
        //     if (mWiFiComDelay.HasFinished())
        //     {
        //         mSerial.Write("AT+CIFSR\r\n");
        //         mExpectedResponse = responseCifsr;
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::WAIT_CIFSR;
        //     }
        // }
        // break;
        // case WIFI_STATE::WAIT_CIFSR:
        // {
        //     if (IsExpectedResponse())
        //     {
        //         mState = WIFI_STATE::LOAD_IP;
        //         IpStringPositionIndex = 0;
        //     }
        //     if (mWiFiComDelay.HasFinished()) 
        //     {
        //         mSerial.Write("AT+CIFSR command not responded ");
        //         mSerial.Write("correctly\r\n");
        //         mState = WIFI_STATE::ERROR;
        //     }
        // }
        // break;
        // case WIFI_STATE::LOAD_IP:
        // {
        //     if (wifi_comComCharRead(&receivedCharWiFiComCom)) 
        //     {
        //         if ((receivedCharWiFiComCom != '"') && (IpStringPositionIndex < IP_MAX_LENGTH) ) 
        //         {
        //             wifi_comComIpAddress[IpStringPositionIndex] = receivedCharWiFiComCom;
        //             IpStringPositionIndex++;
        //         } 
        //         else 
        //         {
        //             wifi_comComIpAddress[IpStringPositionIndex] = '\0';
        //             Util::PcSerialCom::GetInstance()->Write("IP address assigned correctly\r\n\r\n");
        //             mState = WIFI_STATE::SEND_CIPMUX;
        //         }
        //     }
        // }
        // break;
        // case WIFI_STATE::SEND_CIPMUX:
        // {
        //     if (mWiFiComDelay.HasFinished()) 
        //     {
        //         mSerial.Write("AT+CIPMUX=1\r\n");
        //         mExpectedResponse = responseOk;
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::WAIT_CIPMUX;
        //     }
        // }
        // break;
        // case WIFI_STATE::WAIT_CIPMUX:
        // {
        //     if (IsExpectedResponse()) 
        //     {
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::SEND_CIPSERVER;
        //     }
        //     if (mWiFiComDelay.HasFinished()) 
        //     {
        //         Util::PcSerialCom::GetInstance()->Write("AT+CIPMUX=1 command not responded correctly\r\n");
        //         mState = WIFI_STATE::ERROR;
        //     }
        // }
        // break;
        // case WIFI_STATE::SEND_CIPSERVER:
        // {
        //     if (mWiFiComDelay.HasFinished()) 
        //     {
        //         mSerial.Write("AT+CIPSERVER=1,80\r\n");
        //         mExpectedResponse = responseOk;
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::WAIT_CIPSERVER;
        //     }
        // }
        // break;
        // case WIFI_STATE::WAIT_CIPSERVER:
        // {
        //     if (IsExpectedResponse()) 
        //     {
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::SEND_CIPSTATUS;
        //     }

        //     if (mWiFiComDelay.HasFinished()) 
        //     {
        //         Util::PcSerialCom::GetInstance()->Write("AT+CIPSERVER=1,80 command not responded correctly \r\n");
        //         mState = WIFI_STATE::ERROR;
        //     }
        // }
        // break;
        // case WIFI_STATE::SEND_CIPSTATUS:
        // {
        //     if (mWiFiComDelay.HasFinished()) 
        //     {
        //         mSerial.Write("AT+CIPSTATUS\r\n");
        //         mExpectedResponse = responseStatus3;
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::WAIT_CIPSTATUS_STATUS_3;
        //     }
        // }
        // break;
        // case WIFI_STATE::WAIT_CIPSTATUS_STATUS_3:
        // {
        //     if (IsExpectedResponse()) 
        //     {
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mExpectedResponse = responseCipstatus;
        //         mState = WIFI_STATE::WAIT_CIPSTATUS;
        //     }

        //     if (mWiFiComDelay.HasFinished()) 
        //     {
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::SEND_CIPSTATUS;
        //     }
        // }
        // break;

        // case WIFI_STATE::WAIT_CIPSTATUS:
        // {
        //     if (IsExpectedResponse())
        //     {
        //         mState = WIFI_STATE::WAIT_GET_ID;
        //     }
        //     if (mWiFiComDelay.HasFinished())
        //     {
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::SEND_CIPSTATUS;
        //     }
        // }
        // break;
        // case WIFI_STATE::WAIT_GET_ID:
        // {
        //     if(ReadCom(&receivedCharWiFiComCom))
        //     {
        //         currentConnectionId = receivedCharWiFiComCom;
        //         mExpectedResponse = responseOk;
        //         mState = WIFI_STATE::WAIT_CIPSTATUS_OK;
        //     }
        // }
        // break;
        // case WIFI_STATE::WAIT_CIPSTATUS_OK:
        // {
        //     if (IsExpectedResponse()) 
        //     {
        //         mState = WIFI_STATE::SEND_CIPSEND;
        //         wifi_comComWebPageDataUpdate();
        //     }
        //     if (mWiFiComDelay.HasFinished()) 
        //     {
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::SEND_CIPSTATUS;
        //     }
        // }
        // break;
        // case WIFI_STATE::SEND_CIPSEND:
        // {
        //     lengthOfHtmlCode = ( strlen(htmlCodeHeader) 
        //                         + strlen(htmlCodeBody) 
        //                         + strlen(htmlCodeFooter) );
        //     sprintf( strToSend, "AT+CIPSEND=%c,%d\r\n", 
        //             currentConnectionId, lengthOfHtmlCode );
        //     mSerial.Write(strToSend);
        //     mState = WIFI_STATE::WAIT_CIPSEND;
        //     mExpectedResponse = responseOk;
        // }
        // break;
        // case WIFI_STATE::WAIT_CIPSEND:
        // {
        //     if (IsExpectedResponse())
        //     {
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::SEND_HTML;
        //     }

        //     if (mWiFiComDelay.HasFinished()) 
        //     {
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::SEND_CIPSTATUS;
        //     }
        // }
        // break;
        // case WIFI_STATE::SEND_HTML:
        // {
        //     mSerial.Write( htmlCodeHeader );
        //     mSerial.Write( htmlCodeBody );
        //     mSerial.Write( htmlCodeFooter );
        //     mState = WIFI_STATE::WAIT_HTML;
        //     mExpectedResponse = responseSendOk;
        // }
        // break;
        // case WIFI_STATE::WAIT_HTML:
        // {
        //     if (IsExpectedResponse()) 
        //     {
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::SEND_CIPCLOSE;
        //     }
        //     if (mWiFiComDelay.HasFinished()) 
        //     {
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::SEND_CIPSEND;
        //     }
        // }
        // break;
        // case WIFI_STATE::SEND_CIPCLOSE:
        // {
        //     if (mWiFiComDelay.HasFinished()) 
        //     {
        //         sprintf( strToSend, "AT+CIPCLOSE=%c\r\n", currentConnectionId );
        //         mSerial.Write( strToSend );
        //         mExpectedResponse  = responseCipclose;
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::WAIT_CIPCLOSE;
        //     }
        // }
        // break;
        // case WIFI_STATE::WAIT_CIPCLOSE:
        // {
        //     if (IsExpectedResponse())
        //     {
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::SEND_CIPSTATUS;
        //     }

        //     if (mWiFiComDelay.HasFinished()) 
        //     {
        //         mWiFiComDelay.Start(DELAY_5_SECONDS);
        //         mState = WIFI_STATE::SEND_CIPSTATUS;
        //     }
        // }
        // break;
        // case WIFI_STATE::IDLE:
        // case WIFI_STATE::ERROR:
        // break;
    }
}


//=====[Implementations of private functions]==================================

//-----------------------------------------------------------------------------
WiFiCom::WiFiCom(PinName txPin, PinName rxPin, const int baudRate)
    : mSerial(txPin, rxPin, baudRate)
    , mWiFiComDelay(0)
{
    mState = WIFI_STATE::INIT;

    mApSsid = "Cuchitril 2.4GHz";
    mApPassword = "Defensa5232022";
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
bool WiFiCom::ReadCom(char* receivedChar)
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
bool WiFiCom::IsExpectedResponse()
{
    static int responseStringPositionIndex = 0;
    char charReceived;
    bool moduleResponse = false;

    if(ReadCom(&charReceived))
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