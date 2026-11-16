#ifndef XENVASSET_H
#define XENVASSET_H

#include "xBaseAsset.h"

typedef struct p2EnvAsset xEnvAsset;

struct p2EnvAsset : xBaseAsset
{
    U32 bspAssetID;
    U32 startCameraAssetID;
    U32 climateFlags;
    F32 climateStrengthMin;
    F32 climateStrengthMax;
    F32 padF1;
    F32 padF2;
    F32 padF3;
    U32 bspCollisionAssetID;
    U32 bspFXAssetID;
    U32 bspCameraAssetID;
    U32 bspMapperID;
    U32 bspMapperCollisionID;
    U32 bspMapperFXID;
};

#endif
