#ifndef XCLIMATE_H
#define XCLIMATE_H

#include "xMath3.h"

typedef struct p2EnvAsset xEnvAsset;
typedef struct _zParEmitter zParEmitter;

typedef struct _tagRain
{
    S32 rain;
    F32 strength;
    zParEmitter* rain_emitter;
    zParEmitter* snow_emitter;
} Rain;

typedef struct _tagWind
{
    F32 strength;
    F32 angle;
    xVec3 dir;
} Wind;

typedef struct _tagClimate
{
    Rain rain;
    Wind wind;
} Climate;

void xClimateInit(Climate* climate);
void xClimateInitAsset(Climate* climate, xEnvAsset* easset);
void xClimateSetSnow(F32 stre);
void xClimateSetRain(F32 stre);
void xClimateUpdate(Climate* climate, F32 seconds);
void xClimateGetRealWindDir(Climate* climate, xVec3* vec);

#endif
