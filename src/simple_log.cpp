#include <Arduino.h>
#include "simple_log.h"
#define LOGGER_MODULE_NAME "logger"

#define PRINTF(...) Serial.printf(__VA_ARGS__)

struct {
    LOG_LEVEL console_level;
    LOG_LEVEL storage_level;
} slog = {

};

void simple_log_init(LOG_LEVEL console_level, LOG_LEVEL storage_level)
{
    simple_log_set_console_level(console_level);
    simple_log_set_storage_level(storage_level);
}

static const char * level_str[] = {
    "NONE",
    "CRIT",
    "DEBUG",
    "INFO",
    "WARN",
    "ERR",
    "TRACE",
    "MAX"
};

#define now() 0
#define year(t) 0
#define month(t) 0
#define day(t) 0
#define hour(t) 0
#define minute(t) 0
#define second(t) 0

const char *level_to_str(LOG_LEVEL l)
{
    if (l<=LOG_NONE || l>=LOG_MAX)
        return "UNKNOWN";
    return level_str[l];
}

static bool is_valid_level(LOG_LEVEL l)
{
    return l > LOG_NONE && l <LOG_MAX;
}


void simple_log_set_console_level(LOG_LEVEL l)
{
    if(is_valid_level(l))
        slog.console_level = l;
}

void simple_log_set_storage_level(LOG_LEVEL l)
{    
    if(is_valid_level(l))
       slog.storage_level = l;
}

void simple_log(LOG_LEVEL l, const char *module, const char *fmt, ...)
{
    if (slog.console_level > l && slog.storage_level > l)
        return;

    char record[LOGGER_MAX_RECORD_SIZE];
    int record_len=0;
    time_t t = now();
    record_len = sprintf(record, "!%04d-%02d-%02d:%02d:%02d.%02d %02s %02s ", year(t), day(t), month(t), hour(t), minute(t), second(t), level_to_str(l), module);
    va_list args;
    va_start(args, fmt); //Requires the last fixed parameter (to get the address)
    int printed_size = vsnprintf(record+record_len, sizeof(record) - record_len-1, fmt, args);
    va_end(args);
    if (printed_size < 0)
    {
        record_len = snprintf(record+record_len, sizeof(record)-record_len, "log too long", args);
    }
    record_len+=printed_size;
    record[record_len] = '\n';
    record[record_len+1] = '\0';
    if (slog.console_level <= l )
        PRINTF(record);
}
