#ifndef XMOVEPOINT_H
#define XMOVEPOINT_H

#include "xBase.h"
#include "xMovePointAsset.h"

typedef struct _xMovePoint xMovePoint;
struct xSpline3;

typedef struct _xMovePoint : xBase
{
    xMovePointAsset* asset;
    xVec3* pos;
    xMovePoint** nodes;
    xMovePoint* prev;
    U32 node_wt_sum;
    U8 on;
    U8 flg_props;
    U8 pad[2];
    F32 delay;
    xSpline3* spl;
} xMovePoint;

typedef struct _xScene xScene;

void xMovePointInit(xMovePoint* m, xMovePointAsset* asset);
void xMovePointSave(xMovePoint* ent, xSerial* s);
void xMovePointLoad(xMovePoint* ent, xSerial* s);
void xMovePointReset(xMovePoint* m);
void xMovePointSetup(xMovePoint* m, xScene* sc);
void xMovePointSplineSetup(xMovePoint* m);
void xMovePointSplineDestroy(xMovePoint* m);
F32 xMovePointGetNext(xMovePoint* m, xMovePoint* prev, xMovePoint** next, xVec3* hdng);
xVec3* xMovePointGetPos(xMovePoint* m);

#endif
