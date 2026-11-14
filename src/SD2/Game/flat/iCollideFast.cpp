#include "xCommon.h"
#include "iCollideFast.h"

#include <rpcollis.h>

static U32 hitzhit;

void iCollideFastInit(xScene*)
{
}

static RpCollisionTriangle* rayHitsModelFastCB(RpIntersection*, RpCollisionTriangle*, F32, void*)
{
    hitzhit = TRUE;
    return NULL;
}

static RpCollisionTriangle* rayHitsEnvFastCB(RpIntersection*, RpWorldSector*, RpCollisionTriangle*, F32, void*)
{
    hitzhit = TRUE;
    return NULL;
}

U32 iRayHitsModelFast(const xRay3* r, const xModelInstance* m)
{
    RpIntersection isx;

    isx.type = rpINTERSECTLINE;
    xLine3FromRay(*(xLine3*)&isx.t.line, r);

    hitzhit = FALSE;

    RpAtomicForAllIntersections(m->Data, &isx, rayHitsModelFastCB, NULL);
    if (hitzhit) {
        return TRUE;
    }

    RwV3d temp = isx.t.line.start;
    isx.t.line.start = isx.t.line.end;
    isx.t.line.end = temp;

    RpAtomicForAllIntersections(m->Data, &isx, rayHitsModelFastCB, NULL);

    return hitzhit;
}

U32 iRayHitsEnvFast(const xRay3* r, const xEnv* env)
{
    RpIntersection isx;

    isx.type = rpINTERSECTLINE;
    xLine3FromRay(*(xLine3*)&isx.t.line, r);

    hitzhit = FALSE;

    RpCollisionWorldForAllIntersections(env->geom->world, &isx, rayHitsEnvFastCB, NULL);
    if (hitzhit) {
        return TRUE;
    }

    RwV3d temp = isx.t.line.start;
    isx.t.line.start = isx.t.line.end;
    isx.t.line.end = temp;

    RpCollisionWorldForAllIntersections(env->geom->world, &isx, rayHitsEnvFastCB, NULL);

    return hitzhit;
}

U32 iRayHitsEnvSpecialFast(const xRay3* r, const xEnv* env)
{
    RpIntersection isx;

    isx.type = rpINTERSECTLINE;
    xLine3FromRay(*(xLine3*)&isx.t.line, r);

    hitzhit = FALSE;

    RpCollisionWorldForAllIntersections(env->geom->world, &isx, rayHitsEnvFastCB, NULL);
    if (hitzhit) {
        return TRUE;
    }

    if (env->geom->collision) {
        RpCollisionWorldForAllIntersections(env->geom->collision, &isx, rayHitsEnvFastCB, NULL);
    }
    if (hitzhit) {
        return TRUE;
    }

    RwV3d temp = isx.t.line.start;
    isx.t.line.start = isx.t.line.end;
    isx.t.line.end = temp;

    RpCollisionWorldForAllIntersections(env->geom->world, &isx, rayHitsEnvFastCB, NULL);
    if (hitzhit) {
        return TRUE;
    }

    if (env->geom->collision) {
        RpCollisionWorldForAllIntersections(env->geom->collision, &isx, rayHitsEnvFastCB, NULL);
    }

    return hitzhit;
}
