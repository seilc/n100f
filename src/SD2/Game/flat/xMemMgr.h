#ifndef XMEMMGR_H
#define XMEMMGR_H

#include "xDebug.h"

extern U32 gActiveHeap;

void* xMemAlloc(U32 heapID, U32 size);

#endif
