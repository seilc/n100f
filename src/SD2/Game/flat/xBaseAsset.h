#ifndef XBASEASSET_H
#define XBASEASSET_H

#include "types.h"

typedef struct p2BaseAsset xBaseAsset;

struct p2BaseAsset
{
    U32 id;
    U8 baseType;
    U8 linkCount;
    U16 baseFlags;
};

#endif
