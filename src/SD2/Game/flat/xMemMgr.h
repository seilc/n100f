#ifndef XMEMMGR_H
#define XMEMMGR_H

#include "xDebug.h"

struct xMemPool;
typedef void(*xMemPoolInitCallback)(xMemPool*, void*);

struct xMemPool
{
    void* FreeList;
    U16 NextOffset;
    U16 Flags;
    void* UsedList;
    xMemPoolInitCallback InitCB;
    void* Buffer;
    U16 Size;
    U16 NumRealloc;
    U32 Total;
};

extern U32 gActiveHeap;

void* xMemAlloc(U32 heapID, U32 size);

#endif
