#ifndef XBOUND_H
#define XBOUND_H

#include "xMath3.h"
#include "xQuickCull.h"

typedef struct _xCollis xCollis;

enum xBoundType
{
    k_XBOUNDTYPE_NONE,
    k_XBOUNDTYPE_SPHERE,
    k_XBOUNDTYPE_BOX,
    k_XBOUNDTYPE_CYLINDER,
    k_XBOUNDTYPE_OBB,
    k_XBOUNDTYPE_COUNT,
    k_XBOUNDTYPE_SHIFT = 3
};

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

void xRayHitsBound(const xRay3* r, const xQCData* q, const xBound* b, xCollis* c);

#endif
