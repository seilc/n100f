#ifndef XANIM_H
#define XANIM_H

#include "xMemMgr.h"

void xAnimInit();
void xAnimTempTransitionInit(U32 count);
void xAnimPoolInit(xMemPool* pool, U32 count, U32 singles, U32 blendFlags, U32 effectMax);

#endif
