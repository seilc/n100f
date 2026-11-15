#ifndef ZENT_H
#define ZENT_H

#include "xEnt.h"

struct xAnimTable;

typedef struct _zEnt : xEnt
{
    xAnimTable* atbl;
} zEnt;

#endif
