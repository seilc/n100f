#ifndef XMOVEPOINTASSET_H
#define XMOVEPOINTASSET_H

#include "xBaseAsset.h"
#include "xMath3.h"

typedef struct p2MovePointAsset xMovePointAsset;

struct p2MovePointAsset : xBaseAsset
{
    xVec3 pos;
    U16 wt;
    U8 on;
    U8 bezIndex;
    U8 flg_props;
    U8 pad;
    U16 numPoints;
    F32 delay;
};

#endif
