#ifndef XMODEL_H
#define XMODEL_H

#include "xMath3.h"

#include <rpworld.h>

struct xAnimPlay;
struct xModelPool;

struct xModelInstance
{
    xModelInstance* Next;
    xModelInstance* Parent;
    xModelPool* Pool;
    xAnimPlay* Anim;
    RpAtomic* Data;
    void* Object;
    U16 Flags;
    U8 BoneCount;
    U8 BoneIndex;
    U8* BoneRemap;
    RwMatrix* Mat;
    xVec3 Scale;
};

void xModelInit();
void xModelPoolInit(U32 count, U32 numMatrices);

#endif
