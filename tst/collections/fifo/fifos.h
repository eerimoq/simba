#ifndef __FIFOS_H__
#define __FIFOS_H__

#include "simba.h"

/* Declare the integer fifo. */
FIFO_DECLARE_TEMPLATE(int, int);

/* Declare the struct fifo. */
struct foo_t {
    int a;
    int b;
};

FIFO_DECLARE_TEMPLATE(foo, struct foo_t);

/* A fifo of strings. */
typedef char string_t[10];

FIFO_DECLARE_TEMPLATE(strings, string_t);

#endif
