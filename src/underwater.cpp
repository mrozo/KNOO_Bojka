#include "underwater.h"
#include "simple_log.h"
#define LOGGER_MODULE_NAME "underwater"

void underwater_process_start(struct KNOO *KNOO)
{
}

KNOO_MODE underwater_process(struct KNOO *KNOO)
{
    return KNOO_MODE_UNDERWATER;
}

void underwater_process_stop(struct KNOO *KNOO)
{
    
}
