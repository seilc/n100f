#ifndef RWCORE_H
#define RWCORE_H

#include "rwplcore.h"

typedef struct RwBBox RwBBox;
typedef struct RwFrame RwFrame;

struct RwBBox
{
    RwV3d sup;
    RwV3d inf;
};

#endif
