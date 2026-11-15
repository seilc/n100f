#ifndef XCOLLIDE_H
#define XCOLLIDE_H

#include "xMath3.h"
#include "iCollide.h"

typedef struct _xCollis
{
    U32 flags;
    U32 oid;
    void* optr;
    F32 dist;
    xVec3 norm;
    xVec3 tohit;
    xVec3 depen;
    xVec3 hdng;
    iCollis ixtra;
} xCollis;

#endif
