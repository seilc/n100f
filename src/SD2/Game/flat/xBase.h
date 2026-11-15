#ifndef XBASE_H
#define XBASE_H

#include "xBaseAsset.h"
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

// xBase::baseFlags
#define k_XBASE_IS_ENABLED ((U16)(1 << 0))
#define k_XBASE_IS_VALID ((U16)(1 << 2))

struct xSerial;

void xBaseInit(xBase* xb, xBaseAsset* asset);
void xBaseSetup(xBase* xb);
void xBaseSave(xBase* ent, xSerial* s);
void xBaseLoad(xBase* ent, xSerial* s);
void xBaseReset(xBase* xb, xBaseAsset* asset);

inline void xBaseEnable(xBase* xb)
{
    xb->baseFlags |= k_XBASE_IS_ENABLED;
}

inline void xBaseDisable(xBase* xb)
{
    xb->baseFlags &= (U16)~k_XBASE_IS_ENABLED;
}

inline bool xBaseIsEnabled(const xBase* xb)
{
    return xb->baseFlags & k_XBASE_IS_ENABLED;
}

inline void xBaseValidate(xBase* xb)
{
    xb->baseFlags |= k_XBASE_IS_VALID;
}

#endif
