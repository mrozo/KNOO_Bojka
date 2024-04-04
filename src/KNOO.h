#ifndef KNOO_KNOO_H
#define KNOO_KNOO_H
#include "helpers.h"

typedef enum {
    KNOO_MODE_BOOT,
    KNOO_MODE_OPENAIR,
    KNOO_MODE_UNDERWATER,
    KNOO_MODE_MAX,
} KNOO_MODE;

struct KNOO {
    KNOO_MODE mode;
    double pressure;
    double temperature;
    double battery;
};

extern struct KNOO KNOO;

#endif