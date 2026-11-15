#ifndef IMODEL_H
#define IMODEL_H

#include "xMath3.h"

#include <rwcore.h>
#include <rpworld.h>

struct iModelTag
{
    xVec3 v;
    U32 matidx;
    F32 wt[4];
};

void iModelRender(RpAtomic* model, RwMatrix* mat);

#endif
