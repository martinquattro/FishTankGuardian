#ifndef COMMANDS_H
#define COMMANDS_H

// file shared by both projects.
// if a change is made in this file both projects, esp32 and fishTankGuardian, need to be built again

const char COMMAND_CONNECT_STR[]        = "connect";
const char COMMAND_POST_STR[]           = "post";
const char COMMAND_GET_STR[]            = "get";
const char COMMAND_STATUS_STR[]         = "status";
const char COMMAND_ACCESSPOINT_STR[]    = "accesspoint";

const char RESULT_ERROR[]               = "ERROR";
const char RESULT_OK[]                  = "OK";
const char RESULT_CONNECTED[]           = "CONNECTED";
const char RESULT_NOT_CONNECTED[]       = "NOT_CONNECTED";
const char RESULT_AP_WAITING[]          = "AP_WAITING";

const char PARAM_SEPARATOR_CHAR         = '|';
const char STOP_CHAR                    = '~';

const char apSsid[]                  = "FishTankGuardian-AP";
const char apPassword[]              = "Guardian123";
const char guardianHtml[]            = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Fish Tank Guardian</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial, sans-serif; display: inline-block; text-align: center;}
    h1 {font-size: 3.0rem; color: #00bfff;}
    h2 {font-size: 2.0rem; color: #000080;}
    img {width: 50%;}
    input[type=text], input[type=password] {
      width: 50%;
      padding: 12px 20px;
      margin: 8px 0;
      display: inline-block;
      border: 1px solid #ccc;
      box-sizing: border-box;
    }
    input[type=submit] {
      background-color: #4CAF50;
      color: white;
      padding: 14px 20px;
      margin: 8px 0;
      border: none;
      cursor: pointer;
      width: 30%;
    }
  </style>
  </head><body>
  <h1>Fish Tank Guardian</h1>
  <h2>Introduce the WiFi Network you want me to connect</h2> 
  <form action="/connect">
    NETWORK: <input type="text" name="ssid_string"><br>
    PASSWORD: <input type="password" name="pwd_string"><br>
    <input type="submit" value="Connect">
  </form><br>
</body></html>)rawliteral";

#endif