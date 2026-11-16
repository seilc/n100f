#ifndef ZPAREMITTER_H
#define ZPAREMITTER_H

#include "iModel.h"
#include "xBase.h"
#include "xMath3.h"

struct xPar;
struct xParGroup;
struct zParEmitterAsset;

typedef struct _zParEmitter : xBase
{
    zParEmitterAsset* tasset;
    xParGroup* group;
    U8 emit_flags;
    U8 emit_pad[3];
    F32 emit_interval_current;
    U8 rot[3];
    iModelTag tag;
    void* attachTo;
    void* parSys;
    void* emit_volume;
} zParEmitter;

typedef struct _zParEmitterCustomSettings
{
    U32 custom_flags;
    U8 count;
    U8 count_variation;
    F32 interval;
    U32 attachToID;
    xVec3 pos;
    xVec3 vel;
    F32 vel_angle_variation;
    U8 color_birth[4];
    U8 color_death[4];
    F32 size_birth;
    F32 size_birth_variation;
    F32 size_death;
    F32 life;
    F32 life_variation;
    U8 rot[3];
    F32 xzcircle_radius;
    F32 emit_interval_current;
} zParEmitterCustomSettings;

zParEmitter* zParEmitterFind(U32 asset_id);
xPar* zParEmitterEmitCustom(zParEmitter* p, F32 dt, zParEmitterCustomSettings* info);

#endif
