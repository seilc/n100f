#include "xCommon.h"
#include "xMemMgr.h"

#include "iMemMgr.h"
#include "xMath.h"
#include "xutil.h"

#include <stdlib.h>
#include <string.h>

enum
{
    eMemHeapGrowsDown = 0x100,
    eMemHeapHeaderInBlock = 0x8000,
    eMemHeapBlockList = 0x10000,
};

typedef struct xMemBlkInfo_tag
{
    xMemBlock* header;
    U32 pre;
    U32 block;
    U32 post;
    U32 curr;
    U32 waste;
    U32 total;
} xMemBlkInfo;

xMemInfo gMemInfo;
U32 gActiveHeap;
xMemHeap gxHeap[XMEM_MAX_HEAPS];
xMemBaseNotifyFunc sMemBaseNotifyFunc;

static void xMemDebugModeCB()
{
}

void xMemInit()
{
    iMemInit();

    xMemInitHeap(&gxHeap[0], gMemInfo.DRAM.addr, gMemInfo.DRAM.size, 0x8826);
    xMemInitHeap(&gxHeap[1], gMemInfo.DRAM.addr + gMemInfo.DRAM.size, gMemInfo.DRAM.size, 0x8925);
    xMemInitHeap(&gxHeap[2], gMemInfo.DRAM.addr + gMemInfo.DRAM.size, gMemInfo.DRAM.size, 0x892A);

    gxHeap[0].opp_heap[0] = 1;
    gxHeap[0].opp_heap[1] = 2;
    gxHeap[1].opp_heap[0] = 0;
    gxHeap[1].opp_heap[1] = 2;
    gxHeap[2].opp_heap[0] = 0;
    gxHeap[2].opp_heap[1] = 1;

    gActiveHeap = 0;

    xDebugModeRegisterCallback(0xB, xMemDebugModeCB);
}

void xMemExit()
{
    iMemExit();
}

void xMemInitHeap(xMemHeap* heap, U32 base, U32 size, U32 flags)
{
    U32 old_base = base;

    U32 align = 1 << ((flags >> 9) & 0x1F);
    if (flags & eMemHeapGrowsDown) {
        base = old_base & -align;
        size -= old_base - base;
    } else {
        base = -align & (old_base + align - 1);
        size -= base - old_base;
    }

    heap->state_idx = 0;
    heap->hard_base = base;
    heap->size = size;
    heap->flags = flags;
    heap->lastblk = NULL;

    xHeapState* sp = &heap->state[heap->state_idx];
    sp->curr = base;
    sp->blk_ct = 0;

    if (flags & eMemHeapBlockList) {
        heap->max_blks = 1 << ((flags >> 14) & 1);
        heap->blk = (xMemBlock*)malloc(heap->max_blks * sizeof(xMemBlock));
        memset(heap->blk, 0, heap->max_blks * sizeof(xMemBlock));
    } else {
        heap->max_blks = 0xFFFF;
        heap->blk = NULL;
    }

    heap->opp_heap[0] = -1;
    heap->opp_heap[1] = -1;
}

static U32 xMemGetBlockInfo(xMemHeap* heap, U32 size, xMemBlkInfo* info)
{
    U32 align, total;
    S32 hdr, pre, block, post;

    xHeapState* sp = &heap->state[heap->state_idx];
    align = 1 << ((heap->flags >> 9) & 0x1F);
    S32 dir = (heap->flags & eMemHeapGrowsDown) ? -1 : 1;
    S32 header_size = (heap->flags & eMemHeapHeaderInBlock) ? (S32)sizeof(xMemBlock) : 0;
    S32 guard_size = 0;

    hdr = 0;
    pre = (align + header_size - 1) & -align;
    block = pre + ((guard_size + align - 1) & -align);
    post = block + ((size + align - 1) & -align);
    total = post + MAX(align, (guard_size + align - 1) & -align);

    if (heap->flags & eMemHeapGrowsDown) {
        hdr = -header_size;
        pre = -total;
        block = -total + ((guard_size + align - 1) & -align);
        post = block + ((size + align - 1) & -align);
    }

    if (heap->flags & eMemHeapBlockList) {
        info->header = &heap->blk[sp->blk_ct];
    } else {
        info->header = (xMemBlock*)(sp->curr + hdr);
    }

    info->pre = sp->curr + pre;
    info->block = sp->curr + block;
    info->post = sp->curr + post;
    info->curr = sp->curr + dir * total;
    info->waste = total - (header_size + size);
    info->total = total;

    return total;
}

void* xMemGrowAlloc(U32 heapID, U32 size)
{
    U32 align, oldalignsize, newalignsize;
    xMemHeap* heap;
    xMemBlock* hdr;
    xHeapState* sp;
    void* memptr;

    heap = &gxHeap[heapID];
    sp = &heap->state[heap->state_idx];
    hdr = heap->lastblk;
    align = 1 << ((heap->flags >> 9) & 0x1F);

    oldalignsize = (hdr->size + align - 1) & -align;
    newalignsize = (hdr->size + size + align - 1) & -align;

    if (sp->used + (newalignsize - oldalignsize) > heap->size) {
        return NULL;
    }

    if (heap->flags & eMemHeapGrowsDown) {
        memptr = (void*)(hdr->addr - size);
        hdr->addr -= size;
        sp->curr -= newalignsize - oldalignsize;
    } else {
        memptr = (void*)(hdr->addr + hdr->size);
        sp->curr += newalignsize - oldalignsize;
    }

    sp->used += newalignsize - oldalignsize;
    hdr->size += size;
    memset(memptr, 0, size);
    return memptr;
}

void* xMemAlloc(U32 heapID, U32 size)
{
    xMemHeap* heap;
    xMemBlock* hdr;
    xHeapState* sp;
    xMemBlkInfo info;
    U32 i, total;

    heap = &gxHeap[heapID];
    sp = &heap->state[heap->state_idx];

    if (size == 0) {
        return (void*)0xDEADBEEF;
    }

    total = xMemGetBlockInfo(heap, size, &info);
    hdr = info.header;

    if (sp->used + total > heap->size) {
        return NULL;
    }

    for (i = 0; i < ARRAY_LENGTH(heap->opp_heap); i++) {
    }

    sp->curr = info.curr;
    sp->blk_ct++;
    if (heap->flags & eMemHeapBlockList) {
        return NULL;
    }

    hdr->addr = info.block;
    hdr->size = size;

    sp->used += total;
    sp->wasted += info.waste;

    memset((void*)hdr->addr, 0, size);
    heap->lastblk = hdr;

    return (void*)hdr->addr;
}

void* xMemPushTemp(U32 size)
{
    void* addr = malloc(size);
    return addr;
}

void xMemPopTemp(void* addr)
{
    free(addr);
}

S32 xMemPushBase()
{
    xMemHeap* heap = &gxHeap[gActiveHeap];

    heap->state_idx++;
    xHeapState* sp = &heap->state[heap->state_idx];
    *sp = *(sp - 1);

    if (sMemBaseNotifyFunc) {
        sMemBaseNotifyFunc();
    }

    return heap->state_idx - 1;
}

S32 xMemPopBase(S32 depth)
{
    xMemHeap* heap;
    xHeapState* sp;
    S32 new_idx;

    heap = &gxHeap[gActiveHeap];

    if (depth < 0) {
        new_idx = heap->state_idx + depth;
    } else {
        new_idx = depth;
    }

    heap->state_idx = new_idx;
    sp = &heap->state[heap->state_idx];

    if (sMemBaseNotifyFunc) {
        sMemBaseNotifyFunc();
    }

    return heap->state_idx;
}

void xMemRegisterBaseNotifyFunc(xMemBaseNotifyFunc func)
{
    sMemBaseNotifyFunc = func;
}

S32 xMemGetBase()
{
    return gxHeap[gActiveHeap].state_idx;
}

static void xMemPoolAddElements(xMemPool* pool, void* buffer, U32 count)
{
    S32 i;
    void* curr = buffer;
    xMemPoolInitCallback initCB = pool->InitCB;
    U32 next = pool->NextOffset;
    U32 size = pool->Size;

    for (i = 0; i < (S32)count - 1; i++) {
        *(void**)((U8*)curr + next) = (U8*)curr + size;
        if (initCB) initCB(pool, curr);
        curr = (U8*)curr + size;
    }
    *(void**)((U8*)curr + next) = pool->FreeList;
    if (initCB) initCB(pool, curr);

    pool->FreeList = buffer;
    pool->Total += count;
}

void xMemPoolSetup(xMemPool* pool, void* buffer, U32 nextOffset, U32 flags, xMemPoolInitCallback initCB, U32 size, U32 count, U32 numRealloc)
{
    pool->FreeList = NULL;
    pool->NextOffset = nextOffset;
    pool->Flags = flags;
    pool->UsedList = NULL;
    pool->InitCB = initCB;
    pool->Buffer = buffer;
    pool->Size = size;
    pool->NumRealloc = numRealloc;
    pool->Total = 0;

    xMemPoolAddElements(pool, buffer, count);
}

void* xMemPoolAlloc(xMemPool* pool)
{
    void* retval = pool->FreeList;
    U32 next = pool->NextOffset;
    U32 flags = pool->Flags;

    if (!retval) {
        xMemPoolAddElements(pool, xMemAlloc(gActiveHeap, pool->NumRealloc * pool->Size), pool->NumRealloc);
        retval = pool->FreeList;
    }

    pool->FreeList = *(void**)((U8*)retval + next);

    if (flags & 0x1) {
        *(void**)((U8*)retval + next) = pool->UsedList;
        pool->UsedList = retval;
    }

    return retval;
}

void xMemPoolFree(xMemPool* pool, void* data)
{
    if (data) {
        void* freeList = pool->FreeList;
        U32 next = pool->NextOffset;
        U32 flags = pool->Flags;

        if (flags & 0x1) {
            void** prev = &pool->UsedList;
            void* curr = pool->UsedList;
            while (curr && curr != data) {
                prev = (void**)((U8*)curr + next);
                curr = *(void**)((U8*)curr + next);
            }
            if (curr) {
                *prev = *(void**)((U8*)curr + next);
            }
        }

        *(void**)((U8*)data + next) = freeList;
        pool->FreeList = data;
    }
}
