#ifndef KNOO_OPENAIR_H
#define KNOO_OPENAIR_H
#include "KNOO.h"

void openair_process_start(struct KNOO *KNOO);
KNOO_MODE openair_process(struct KNOO *KNOO);
void openair_process_stop(struct KNOO *KNOO);

#endif