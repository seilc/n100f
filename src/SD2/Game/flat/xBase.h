#ifndef XBASE_H
#define XBASE_H

#include "xDebug.h"

typedef struct p2LinkAsset xLinkAsset;

struct xBase;

typedef S32(*xBaseEventCallback)(xBase* from, xBase* to, U32 toEvent, F32* toParam, xBase* toParamWidget);

struct xBase
{
    U32 id;
    U8 baseType;
    U8 linkCount;
    U16 baseFlags;
    const xLinkAsset* link;
    xBaseEventCallback eventFunc;
};

struct xSerial;

void xBaseSave(xBase* ent, xSerial* s);
void xBaseLoad(xBase* ent, xSerial* s);

#endif
