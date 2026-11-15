#ifndef XBOUND_H
#define XBOUND_H

#include "xMath3.h"
#include "xQuickCull.h"

typedef struct _xBound
{
    U8 type;
    U8 pad[3];
    union
    {
        xSphere sph;
        xBBox box;
        xCylinder cyl;
    };
    xMat4x3* mat;
    xQCData qcd;
} xBound;

#endif
