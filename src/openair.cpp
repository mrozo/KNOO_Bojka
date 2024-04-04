#include "openair.h"
#include <Arduino.h>
#include <WiFi.h>
#include "simple_log.h"
#define LOGGER_MODULE_NAME "openair"

void openair_process_start(struct KNOO *KNOO)
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP("KNOO_BOJA", "dupaDupa");
}

KNOO_MODE openair_process(struct KNOO *KNOO)
{
    return KNOO_MODE_OPENAIR;
}

void openair_process_stop(struct KNOO *KNOO)
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}
