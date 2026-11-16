#include "xCommon.h"
#include "xClimate.h"

#include "iMath.h"
#include "xEnvAsset.h"
#include "xMath3.h"
#include "xModel.h"
#include "xString.h"
#include "xutil.h"
#include "zGlobals.h"
#include "zParEmitter.h"

static Climate* sClimate;

static void xClimateVecFromAngle(F32 angle, xVec3* vec)
{
    xMat3x3 mat;
    xMat3x3Identity(&mat);
    xMat3x3Euler(&mat, PI * angle / 180.0f, 0.0f, 0.0f);
    xVec3Init(vec, 0.0f, 0.0f, 1.0f);
    xMat3x3LMulVec(vec, &mat, vec);
}

void xClimateInit(Climate* climate)
{
    climate->rain.strength = 0.0f;
    climate->rain.rain_emitter = zParEmitterFind(xStrHash("PAREMIT_RAIN"));
    climate->rain.rain_emitter->emit_flags &= (U8)~0x1;
    climate->rain.snow_emitter = zParEmitterFind(xStrHash("PAREMIT_SNOW"));
    climate->rain.snow_emitter->emit_flags &= (U8)~0x1;
}

void xClimateInitAsset(Climate* climate, xEnvAsset* easset)
{
    Wind* w = &climate->wind;
    sClimate = climate;
    w->strength = 0.0f;
    xClimateVecFromAngle(w->angle, &w->dir);

    if (easset->climateFlags == 0) {
        w->strength = 0.0f;
        climate->rain.strength = 0.0f;
    } else if (easset->climateFlags & 0x1) {
        climate->rain.rain = TRUE;
        climate->rain.strength = 0.5f * (easset->climateStrengthMax - easset->climateStrengthMin);
        climate->rain.strength += easset->climateStrengthMin;
    } else if (easset->climateFlags & 0x2) {
        climate->rain.rain = FALSE;
        climate->rain.strength = 0.5f * (easset->climateStrengthMax - easset->climateStrengthMin);
        climate->rain.strength += easset->climateStrengthMin;
    }
}

void xClimateSetSnow(F32 stre)
{
    sClimate->rain.rain = FALSE;
    sClimate->rain.strength = stre;
}

void xClimateSetRain(F32 stre)
{
    sClimate->rain.rain = TRUE;
    sClimate->rain.strength = stre;
}

static void GetPosBigDogWhattupFool(xVec3* pos)
{
    xMat4x3* mat = &globals.camera.hi_cam.mat;
    pos->x = 10.0f * mat->at.x + mat->pos.x;
    pos->y = 10.0f * mat->at.y + mat->pos.y;
    pos->z = 10.0f * mat->at.z + mat->pos.z;
}

static void UpdateRain(Climate* climate, F32 seconds)
{
    zParEmitterCustomSettings info;
    info.custom_flags = 0x20;

    if (climate->rain.rain) {
        S32 total_rain_drops = 25.0f * climate->rain.strength;
        if (globals.sceneCur->sceneID == SCENE_ID("R004")) {
            for (S32 i = 0; i < 5; i++) {
                xVec3Init(&info.pos, 99.9643f, 32.8615f, -177.306f);
                info.pos.x += 25.0f * iurand() - 12.5f;
                info.pos.y += 10.0f;
                info.pos.z += 25.0f * iurand() - 12.5f;
                zParEmitterEmitCustom(climate->rain.rain_emitter, seconds, &info);
            }
            for (S32 i = 0; i < 5; i++) {
                xVec3Copy(&info.pos, (xVec3*)&globals.player.ent.model->Mat->pos);
                info.pos.x += 25.0f * iurand() - 12.5f;
                info.pos.y += 10.0f;
                info.pos.z += 25.0f * iurand() - 12.5f;
                zParEmitterEmitCustom(climate->rain.rain_emitter, seconds, &info);
            }
            for (S32 i = 0; i < total_rain_drops; i++) {
                GetPosBigDogWhattupFool(&info.pos);
                info.pos.x += 25.0f * iurand() - 12.5f;
                info.pos.y += 10.0f;
                info.pos.z += 25.0f * iurand() - 12.5f;
                zParEmitterEmitCustom(climate->rain.rain_emitter, seconds, &info);
            }
        } else {
            for (S32 i = 0; i < total_rain_drops; i++) {
                GetPosBigDogWhattupFool(&info.pos);
                info.pos.x += 25.0f * iurand() - 12.5f;
                info.pos.y += 10.0f;
                info.pos.z += 25.0f * iurand() - 12.5f;
                zParEmitterEmitCustom(climate->rain.rain_emitter, seconds, &info);
            }
        }
    } else {
        S32 total_snow_flakes = 25.0f * climate->rain.strength;
        info.custom_flags |= 0x1;
        xVec3 player_pos;
        GetPosBigDogWhattupFool(&player_pos);

        for (S32 i = 0; i < total_snow_flakes; i++) {
            info.pos = player_pos;
            info.pos.x += 45.0f * iurand() - 22.5f;
            info.pos.z += 45.0f * iurand() - 22.5f;
            // BUG: x times z, not the squared distance
            F32 perc = (info.pos.x - player_pos.x) * (info.pos.z - player_pos.z) / 506.25f;
            perc = 1.0f - perc;
            info.pos.y += 5.0f * perc + 5.0f;
            info.life = 2.5f * perc + 2.5f;
            zParEmitterEmitCustom(climate->rain.snow_emitter, seconds, &info);
        }
    }
}

void xClimateUpdate(Climate* climate, F32 seconds)
{
    UpdateRain(climate, seconds);
}

void xClimateGetRealWindDir(Climate* climate, xVec3* vec)
{
    vec->x = climate->wind.dir.x * climate->wind.strength;
    vec->y = 0.0f;
    vec->z = climate->wind.dir.z * climate->wind.strength;
}
