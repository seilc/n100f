#ifndef XCOLLIDE_H
#define XCOLLIDE_H

#include "xMath3.h"
#include "iCollide.h"

typedef struct _xScene xScene;

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

typedef enum _xCollsIdx
{
    k_XCOLLS_IDX_FLOOR,
    k_XCOLLS_IDX_CEIL,
    k_XCOLLS_IDX_FRONT,
    k_XCOLLS_IDX_LEFT,
    k_XCOLLS_IDX_REAR,
    k_XCOLLS_IDX_RIGHT,
    k_XCOLLS_IDX_COUNT
} xCollsIdx;

// xCollis::flags
#define k_HIT_IT ((U32)(1 << 0))
#define k_HIT_CALC_HDNG ((U32)(1 << 12))

xCollsIdx xCollideGetCollsIdx(const xVec3* tohit, const xMat3x3* mat);
void xCollideInit(xScene* sc);
U32 xSphereHitsSphere(const xSphere* a, const xSphere* b, xCollis* coll);
U32 xSphereHitsBox(const xSphere* a, const xBox* b, xCollis* coll);
U32 xSphereHitsOBB(const xSphere* a, const xBox* b, const xMat4x3* m, xCollis* coll);

#endif
