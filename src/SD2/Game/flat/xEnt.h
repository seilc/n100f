#ifndef XENT_H
#define XENT_H

#include "xBase.h"
#include "xBound.h"
#include "iShadow.h"

typedef struct p2EntAsset xEntAsset;
typedef struct _xEnt xEnt;
typedef struct _xEntCollis xEntCollis;
typedef struct _xEntFrame xEntFrame;
typedef struct _xEntShadow xEntShadow;
typedef struct _xScene xScene;
struct xFFX;
struct xModelInstance;
struct xSurface;

typedef void(*xEntUpdateCallback)(xEnt*, xScene*, F32);
typedef void(*xEntBoundUpdateCallback)(xEnt*, xVec3*);
typedef void(*xEntMoveCallback)(xEnt*, xScene*, F32, xEntFrame*);
typedef void(*xEntRenderCallback)(xEnt*);

// xEnt::collLev
#define k_XENT_COLLLEV_FASTEST 0
#define k_XENT_COLLLEV_AABB 1
#define k_XENT_COLLLEV_SPH 2
#define k_XENT_COLLLEV_OBB 3
#define k_XENT_COLLLEV_BOUND 4
#define k_XENT_COLLLEV_MODEL 5

typedef struct _xEnt : xBase
{
    xEntAsset* asset;
    U8 flags;
    U8 subType;
    U8 pflags;
    U8 moreFlags;
    U8 pad0[3];
    U8 num_ffx;
    U16 idx;
    U16 num_updates;
    F32 material_alpha;
    F32 fade_tmr;
    U8 collType;
    U8 collLev;
    U8 chkby;
    U8 penby;
    xModelInstance* model;
    xEntUpdateCallback update;
    xEntBoundUpdateCallback bupdate;
    xEntMoveCallback move;
    xEntRenderCallback render;
    xEntFrame* frame;
    xEntCollis* collis;
    xBound bound;
    F32 updateTimer;
    F32 updateAcc;
    xEntRenderCallback updateTimerFunc;
    xEntBoundUpdateCallback transl;
    iShadowCache ishadow;
    xSurface* surf;
    xFFX* ffx;
    xEnt* driver;
    xEntShadow* ent_shadow;
} xEnt;

#endif
