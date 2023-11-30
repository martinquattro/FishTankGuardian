#ifndef TELEGRAM_BOT_LIB_H
#define TELEGRAM_BOT_LIB_H

const char COMMAND_START[]                      = "/start";
const char COMMAND_FEEDER_FEED[]                = "/feeder_feed";
const char COMMAND_FEEDER_STATUS[]              = "/feeder_status";
const char COMMAND_FEEDER_SET[]                 = "/feeder_set";
const char COMMAND_FEEDER_DELETE[]              = "/feeder_delete";
const char COMMAND_TIMEZONE[]                   = "/timezone";
const char COMMAND_MONITOR_STATUS[]             = "/monitor_status";
const char COMMAND_MONITOR_SET_TEMP_LIMITS[]    = "/monitor_set_temp_limits";
const char COMMAND_MONITOR_SET_TDS_LIMITS[]     = "/monitor_set_tds_limits";

const char ERROR_INVALID_COMMAND[]          = "[ERROR]\nInvalid command [%s]";
const char ERROR_INVALID_PARAMETERS[]       = "[ERROR]\nInvalid parameters for [%s] command.";
const char ERROR_INVALID_USER[]             = "[ERROR]\nInvalid user";

const char ALERT_OUT_OF_LIMITS[]            = "[ALERT]\nOne of the sensors is out of the set limits";

#endif // TELEGRAM_BOT_LIB_H