#ifndef KNOO_SIMPLE_LOG_H
#define KNOO_SIMPLE_LOG_H
#ifndef LOGGER_MAX_RECORD_SIZE
#define LOGGER_MAX_RECORD_SIZE (200)
#endif
typedef enum
{
    LOG_NONE,
    LOG_CRIT,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERR,
    LOG_TRACE,
    LOG_MAX,
}LOG_LEVEL;

void simple_log_init(LOG_LEVEL console_level, LOG_LEVEL storage_level);

void simple_log(LOG_LEVEL l, const char *module, const char *fmt, ...);

void simple_log_set_console_level(LOG_LEVEL l);
void simple_log_set_storage_level(LOG_LEVEL l);

#define CRIT(...)  simple_log(LOG_CRIT, LOGGER_MODULE_NAME, __VA_ARGS__);
#define INFO(...)  simple_log(LOG_INFO, LOGGER_MODULE_NAME, __VA_ARGS__);
#define WARN(...)  simple_log(LOG_WARN, LOGGER_MODULE_NAME, __VA_ARGS__);
#define DBG(...)   simple_log(LOG_DEBUG, LOGGER_MODULE_NAME, __VA_ARGS__);
#define ERR(...)   simple_log(LOG_ERR, LOGGER_MODULE_NAME, __VA_ARGS__);
#define TRACE(...) simple_log(LOG_TRACE, LOGGER_MODULE_NAME, __VA_ARGS__);
#endif