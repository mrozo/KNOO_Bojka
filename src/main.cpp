#include <Arduino.h>
#include "KNOO.h"
#include "console.h"
#include "underwater.h"
#include "openair.h"
#include "simple_log.h"
#define LOGGER_MODULE_NAME "main"

struct KNOO KNOO = {
  .mode = KNOO_MODE_BOOT,
};

void setup() {
  Serial.begin(115200);
  simple_log_init(LOG_INFO, LOG_INFO);
}

void loop() {
  KNOO_MODE new_mode;
  switch(KNOO.mode)
  {
    case KNOO_MODE_BOOT:       new_mode = KNOO_MODE_OPENAIR; break;
    case KNOO_MODE_OPENAIR:    new_mode = openair_process(&KNOO); break;
    case KNOO_MODE_UNDERWATER: new_mode = underwater_process(&KNOO); break;
    default:
      CRIT("unknow mode %d", KNOO.mode);
      ESP.restart();
  }

  if (new_mode != KNOO.mode)
  {
    if(KNOO.mode == KNOO_MODE_OPENAIR)
      openair_process_stop(&KNOO);
    if(KNOO.mode == KNOO_MODE_UNDERWATER)
      underwater_process_stop(&KNOO);
    if(new_mode == KNOO_MODE_OPENAIR)
      openair_process_start(&KNOO);
    if(new_mode == KNOO_MODE_UNDERWATER)
      underwater_process_start(&KNOO);
  }

  KNOO.mode = new_mode;
  console_process();
}
