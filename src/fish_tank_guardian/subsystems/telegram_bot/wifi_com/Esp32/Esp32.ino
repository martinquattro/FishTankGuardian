#include <Arduino.h>
#include <map>
#include <vector>
#include <functional>
#include <WiFi.h>
#include <HTTPClient.h>
#include "commands.h"

#define SERIAL_PRINT(format, ...) \
    do { \
        char buffer[256]; \
        snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
        Serial.println(buffer); \
    } while (0)

// Redefine name for nucleo board serial communication
HardwareSerial &NucleoBoardCom = Serial2;

// Map of the possible commands and their associated function
using CommandFunction = std::function<String(const std::vector<String>&)>;
std::map<String, CommandFunction> commandsMap;

// Functions declarations
std::vector<String> ParseParameters(const String &input);
String CommandConnectToWiFi(const std::vector<String>& params);
String CommandPostToServer(const std::vector<String>& params);
String CommandGet(const std::vector<String>& params);

// ---------------------------------------------------------------------------------------
void setup() 
{
    Serial.begin(115200);
    NucleoBoardCom.begin(115200);

    // Asignar las funciones al mapa
    commandsMap[COMMAND_CONNECT_STR]    = CommandConnectToWiFi;
    commandsMap[COMMAND_POST_STR]       = CommandPostToServer;
    commandsMap[COMMAND_GET_STR]        = CommandGet;
}

// ---------------------------------------------------------------------------------------
void loop()
{
    if (NucleoBoardCom.available()) 
    {
        String commandExecutionResult;

        String strReceived = NucleoBoardCom.readString();
        strReceived.trim();

        std::vector<String> params = ParseParameters(strReceived);
        String cmd = params[0];

        if (commandsMap.find(cmd) != commandsMap.end()) 
        {
            CommandFunction commandFunction = commandsMap[cmd];
            commandExecutionResult = commandFunction(params);
        } 
        else 
        {
            SERIAL_PRINT("Command [%s] not found", cmd.c_str());
            commandExecutionResult = RESULT_ERROR;
        }

        NucleoBoardCom.println(commandExecutionResult);
    }
}

// ---------------------------------------------------------------------------------------
String CommandConnectToWiFi(const std::vector<String>& params)
{
    if (params.size() == 3) 
    {
        String ssid = params[1];
        String password = params[2];

        WiFi.begin(ssid.c_str(), password.c_str());

        SERIAL_PRINT("CommandConnectToWiFi - Connecting to WiFi: [%s]", ssid);

        int attempts = 0;

        while (WiFi.status() != WL_CONNECTED && attempts < 30) 
        {
            delay(1000);
            Serial.print(".");
            attempts++;
        }

        if (WiFi.status() == WL_CONNECTED) 
        {
            SERIAL_PRINT("\nCommandConnectToWiFi - Connected to IP: [%s]", WiFi.localIP());
            return RESULT_OK;
        } 
        else 
        {
            Serial.println("\nCommandConnectToWiFi - Error while connecting");
            return RESULT_ERROR;
        }
    } 
    else 
    {
        SERIAL_PRINT("CommandConnectToWiFi- Incorrect amount of parameters [%d]", (params.size() - 1));
        return RESULT_ERROR;
    }
}

// ---------------------------------------------------------------------------------------
String CommandPostToServer(const std::vector<String>& params)
{
    if (params.size() == 3) 
    {
        String server = params[1];
        String request = params[2];

        SERIAL_PRINT("CommandPostToServer- Post to server = [%s]\n%s", server.c_str(), request.c_str());
        
        HTTPClient http;

        http.begin(server.c_str());
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        int httpResponseCode = http.POST(request);
        String response;

        if (httpResponseCode > 0) 
        {
            SERIAL_PRINT("CommandPostToServer - Success\n[%d]\n[%s]", httpResponseCode, http.getString().c_str());
            response = http.getString();
        } 
        else 
        {
            response = RESULT_ERROR;
        }

        http.end();

        return response;
    } 
    else 
    {
        SERIAL_PRINT("CommandPostToServer - Incorrect amount of parameters [%d]", (params.size() - 1));
        return RESULT_ERROR;
    }
}

// ---------------------------------------------------------------------------------------
String CommandGet(const std::vector<String>& params)
{
    if (params.size() == 2) 
    {
        String url = params[1];

        HTTPClient http;
        http.begin(url.c_str());

        int httpResponseCode = http.GET();
        String response;

        if (httpResponseCode > 0) 
        {
            SERIAL_PRINT("CommandGet - Success\n[%d]\n[%s]", httpResponseCode, http.getString().c_str());
            response = http.getString();
        } 
        else 
        {
            response = RESULT_ERROR;
        }

        http.end();

        return response;
    } 
    else 
    {
        SERIAL_PRINT("CommandGet - Incorrect amount of parameters [%d]", (params.size() - 1));
        return RESULT_ERROR;
    }
}

// ---------------------------------------------------------------------------------------
std::vector<String> ParseParameters(const String &input) 
{
    size_t parameters_size = 0;
    int index_from = -1, index_to;
    
    if (input.length() > 0) 
    {
        parameters_size++;
    }

    while ((index_from = input.indexOf(PARAM_SEPARATOR, index_from + 1)) >= 0) 
    {
        parameters_size++;
    }

    std::vector<String> params(parameters_size);

    index_from = -1;
    for (size_t i = 0; i < parameters_size; i++) 
    {
        index_to = input.indexOf(PARAM_SEPARATOR, index_from + 1);
        
        if (index_to < 0)
        index_to = input.length();

        params[i] = input.substring(index_from + 1, index_to);
        index_from = index_to;
    }

    return params;
}