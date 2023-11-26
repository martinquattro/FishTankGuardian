#ifndef COMMANDS_H
#define COMMANDS_H

// file shared by both projects.
// if a change is made in this file both projects, esp32 and fishTankGuardian, need to be built again

const char COMMAND_CONNECT_STR[]    = "connect";
const char COMMAND_POST_STR[]       = "post";
const char COMMAND_GET_STR[]        = "get";
const char COMMAND_STATUS_STR[]     = "status";

const char RESULT_ERROR[]               = "ERROR";
const char RESULT_OK[]                  = "OK";
const char RESULT_CONNECTED[]           = "CONNECTED";
const char RESULT_NOT_CONNECTED[]       = "NOT_CONNECTED";

const char PARAM_SEPARATOR_CHAR         = '|';
const char STOP_CHAR                    = '~';

#endif