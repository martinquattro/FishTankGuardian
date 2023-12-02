#include <Arduino.h>
#include <functional>
#include <HTTPClient.h>
#include <map>
#include <vector>
#include <WiFi.h>
#include <WebServer.h>

#include "commands.h"

#define RXD2 16
#define TXD2 17
#define LED_WIFI_STATUS 2

#define DEBUG_PRINTLN(format, ...) \
    do { \
        char buffer[256]; \
        snprintf(buffer, sizeof(buffer), format, ##__VA_ARGS__); \
        Serial.println(buffer); \
    } while (0)

// Map of the possible commands and their associated function
using CommandFunction = std::function<String(const std::vector<String>&)>;
std::map<String, CommandFunction> commandsMap;

// Functions declarations
String CommandConnectToWiFi(const std::vector<String>& params);
String CommandPostToServer(const std::vector<String>& params);
String CommandGet(const std::vector<String>& params);
String CommandStatus(const std::vector<String>& params);

std::vector<String> _ParseParameters(const String &input);
bool _IsConnected();

// ---------------------------------------------------------------------------------------
void setup() 
{
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

    pinMode(LED_WIFI_STATUS,OUTPUT);

    commandsMap[COMMAND_CONNECT_STR]        = CommandConnectToWiFi;
    commandsMap[COMMAND_POST_STR]           = CommandPostToServer;
    commandsMap[COMMAND_GET_STR]            = CommandGet;
    commandsMap[COMMAND_STATUS_STR]         = CommandStatus;
    commandsMap[COMMAND_ACCESSPOINT_STR]    = CommandAccessPoint;
}

// ---------------------------------------------------------------------------------------
void loop()
{
    digitalWrite(LED_WIFI_STATUS, (_IsConnected()) ? HIGH : LOW);

    if (Serial2.available()) 
    {
        String commandExecutionResult;

        String strReceived = Serial2.readStringUntil(STOP_CHAR);
        strReceived.trim();
        DEBUG_PRINTLN("Command and parameters received [%s]", strReceived.c_str());

        std::vector<String> params = _ParseParameters(strReceived);
        String cmd = params[0];

        if (commandsMap.find(cmd) != commandsMap.end()) 
        {
            CommandFunction commandFunction = commandsMap[cmd];
            commandExecutionResult = commandFunction(params);

            Serial2.flush();
            Serial2.print(commandExecutionResult.c_str());
            Serial2.print(STOP_CHAR);

            DEBUG_PRINTLN("Result = [%s] sent to Nucleo Board", commandExecutionResult.c_str());
        } 
        else 
        {
            DEBUG_PRINTLN("Command [%s] not found", cmd.c_str());
        }
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

        DEBUG_PRINTLN("CommandConnectToWiFi - Connecting to WiFi: [%s]", ssid.c_str());

        int attempts = 0;

        while ((WiFi.status() != WL_CONNECTED) && (attempts < 10)) 
        {
            delay(1000);
            Serial.print(".");
            attempts++;
        }

        if (WiFi.status() == WL_CONNECTED) 
        {
            DEBUG_PRINTLN("\nCommandConnectToWiFi - Connected to IP: [%s]", WiFi.localIP().toString().c_str());
            return RESULT_OK;
        } 
        else 
        {
            DEBUG_PRINTLN("CommandConnectToWiFi - Error while connecting");
            return RESULT_ERROR;
        }
    } 
    else 
    {
        DEBUG_PRINTLN("CommandConnectToWiFi- Incorrect amount of parameters [%d]", (params.size() - 1));
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

        if (!_IsConnected()) 
        {
            DEBUG_PRINTLN("CommandPostToServer - No Connection to WiFi");
            return RESULT_ERROR;
        }

        DEBUG_PRINTLN("CommandPostToServer - Post to server = [%s]\n%s", server.c_str(), request.c_str());
        
        HTTPClient http;

        http.begin(server.c_str());
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        int httpResponseCode = http.POST(request);
        String response;

        if (httpResponseCode > 0) 
        {
            response = http.getString();
            DEBUG_PRINTLN("CommandPostToServer - Success\n[%d]\n[%s]", httpResponseCode, response.c_str());
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
        DEBUG_PRINTLN("CommandPostToServer - Incorrect amount of parameters [%d]", (params.size() - 1));
        return RESULT_ERROR;
    }
}

// ---------------------------------------------------------------------------------------
String CommandGet(const std::vector<String>& params)
{
    if (params.size() == 2) 
    {
        if (!_IsConnected()) 
        {
            DEBUG_PRINTLN("CommandGet - No Connection to WiFi");
            return RESULT_ERROR;
        }

        String url = params[1];

        HTTPClient http;
        http.begin(url.c_str());

        int httpResponseCode = http.GET();
        String response;

        if (httpResponseCode > 0) 
        {
            response = http.getString();
            DEBUG_PRINTLN("CommandGet - Success\n[%d]\n[%s]", httpResponseCode, response.c_str());
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
        DEBUG_PRINTLN("CommandGet - Incorrect amount of parameters [%d]", (params.size() - 1));
        return RESULT_ERROR;
    }
}

// ---------------------------------------------------------------------------------------
String CommandStatus(const std::vector<String>& params)
{
    if (params.size() == 1) 
    {
        return (_IsConnected()) ? (RESULT_CONNECTED) : (RESULT_NOT_CONNECTED);
    }
    else
    {
        DEBUG_PRINTLN("CommandStatus - Incorrect amount of parameters [%d]", (params.size() - 1));
        return RESULT_ERROR;
    }
}

// ---------------------------------------------------------------------------------------
String CommandAccessPoint(const std::vector<String>& params)
{
    static bool formSubmitted = false;
    static String network = "";
    static String password = "";

    if (params.size() == 1) 
    {
        WiFi.softAP(apSsid, apPassword);

        WiFiServer server(80);
        server.begin();

        DEBUG_PRINTLN("Access Point configured. Connect to WiFi network: %s | Password: %s", apSsid, apPassword);
        DEBUG_PRINTLN("Open a web browser and go to http://%s/ to view the HTML page", WiFi.softAPIP().toString().c_str());

        // Bucle principal para manejar múltiples clientes
        while (true)
        {
            WiFiClient client = server.available();
            if (client)
            {
                DEBUG_PRINTLN("Client Connected");
                
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println();
                client.println(guardianHtml);

                // Procesar todas las solicitudes disponibles
                while (client.connected() && client.available())
                {
                    String request = client.readStringUntil('\r');
                    if (request.indexOf("/connect") != -1)
                    {
                       
                        network = _GetValueFromRequest(request, "ssid_string");
                        password = _GetValueFromRequest(request, "pwd_string");

                        // Cierra la conexión después de procesar la solicitud
                        // client.stop();

                        return (network + PARAM_SEPARATOR_CHAR + password);
                    }
                }

                // Descartar las líneas adicionales después de procesar la solicitud
                while (client.available() && client.read() != -1) {}

                // Cierra la conexión si no se procesó ninguna solicitud
                client.stop();
                DEBUG_PRINTLN("Client Disconnected");
            }
        }
    }
    else 
    {
        DEBUG_PRINTLN("CommandAccessPoint- Incorrect amount of parameters [%d]", (params.size() - 1));
        return RESULT_ERROR;
    }
}

// ---------------------------------------------------------------------------------------
String _GetValueFromRequest(String data, String key)
{
    String value = "";
    int index = data.indexOf(key + "=");
    if (index != -1)
    {
        index += key.length() + 1;
        int endIndex = data.indexOf("&", index);
        
        if (endIndex == -1)
        {
            endIndex = data.indexOf(" ", index);
            if (endIndex == -1)
            {
                endIndex = data.length();
            }
        }
        
        value = data.substring(index, endIndex);
        value.replace("+", " ");
    }

    return value;
}

// ---------------------------------------------------------------------------------------
bool _IsConnected()
{
    return (WiFi.status() == WL_CONNECTED);
}
// ---------------------------------------------------------------------------------------
std::vector<String> _ParseParameters(const String &input) 
{
    size_t parameters_size = 0;
    int index_from = -1, index_to;

    if (input.length() > 0) 
    {
        parameters_size++;
    }

    while ((index_from = input.indexOf(PARAM_SEPARATOR_CHAR, index_from + 1)) >= 0) 
    {
        parameters_size++;
    }

    std::vector<String> params(parameters_size);

    index_from = -1;
    for (size_t i = 0; i < parameters_size; i++) 
    {
        index_to = input.indexOf(PARAM_SEPARATOR_CHAR, index_from + 1);
        
        if (index_to < 0)
        index_to = input.length();

        params[i] = input.substring(index_from + 1, index_to);
        index_from = index_to;
    }

    return params;
}