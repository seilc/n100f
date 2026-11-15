#include "xCommon.h"
#include "xCollideFast.h"

#include "xBound.h"
#include "xScene.h"
#include "iCollideFast.h"

void xCollideFastInit(xScene* sc)
{
    iCollideFastInit(sc);
}

U32 xRayHitsSphereFast(const xRay3* r, const xSphere* s)
{
    xVec3 diff;
    xVec3Sub(&diff, &r->origin, &s->center);

    F32 c = xVec3Dot(&diff, &diff) - s->r * s->r;
    if (c <= 0.0f) {
        return TRUE;
    }

    if (r->flags & XRAY3_USE_MAX) {
        if (c > (2.0f * s->r + r->max_t) * r->max_t) {
            return FALSE;
        }
    }

    F32 b = xVec3Dot(&diff, &r->dir);
    if (b >= 0.0f) {
        return FALSE;
    }

    return (b * b >= c);
}

U32 xRayHitsBoxFast(const xRay3* r, const xBox* b)
{
    xIsect isx;
    iBoxIsectRay(b, r, &isx);

    return (isx.penned <= 0.0f || isx.contained <= 0.0f);
}

U32 xRayHitsCylinderFast(const xRay3* r, const xCylinder* c)
{
    xIsect isx;
    iCylinderIsectRay(c, r, &isx);

    return (isx.penned <= 0.0f || isx.contained <= 0.0f);
}

U32 xRayHitsBoundFast(const xRay3* r, const xBound* b)
{
    if (b->type == k_XBOUNDTYPE_SPHERE) {
        return xRayHitsSphereFast(r, &b->sph);
    } else if (b->type == k_XBOUNDTYPE_OBB) {
        xRay3 lr;
        xMat3x3 mn;
        F32 ms = xVec3Length(&b->mat->right);

        xMat3x3Normalize(&mn, (xMat3x3*)b->mat);
        xMat4x3ToLocal(&lr.origin, b->mat, &r->origin);
        xMat3x3Tolocal(&lr.dir, &mn, &r->dir);
        lr.max_t = r->max_t / ms;
        lr.min_t = r->min_t / ms;
        lr.flags = r->flags;

        return xRayHitsBoxFast(&lr, &b->box.box);
    } else if (b->type == k_XBOUNDTYPE_BOX) {
        return xRayHitsBoxFast(r, &b->box.box);
    } else if (b->type == k_XBOUNDTYPE_CYLINDER) {
        return xRayHitsCylinderFast(r, &b->cyl);
    }

    return FALSE;
}

U32 xRayHitsEnvFast(const xRay3* r, const xEnv* env)
{
    return iRayHitsEnvFast(r, env);
}

U32 xRayHitsSceneFast(const xRay3* r, xScene* sc)
{
    xQCData qcr;
    U32 hit;
    S32 i;

    xQuickCullForRay(&qcr, r);

    for (i = 0; i < sc->num_stats; i++) {
        xEnt* ent = sc->stats[i];
        if (ent->chkby && xQuickCullIsects(&qcr, &ent->bound.qcd) && xRayHitsBoundFast(r, &ent->bound)) {
            if (ent->collLev == k_XENT_COLLLEV_MODEL) {
                hit = iRayHitsModelFast(r, ent->model);
                if (hit) {
                    return TRUE;
                }
            } else {
                return TRUE;
            }
        }
    }

    for (i = 0; i < sc->num_dyns; i++) {
        xEnt* ent = sc->dyns[i];
        if (ent->chkby && xQuickCullIsects(&qcr, &ent->bound.qcd) && xRayHitsBoundFast(r, &ent->bound)) {
            if (ent->collLev == k_XENT_COLLLEV_MODEL) {
                hit = iRayHitsModelFast(r, ent->model);
                if (hit) {
                    return TRUE;
                }
            } else {
                return TRUE;
            }
        }
    }

    for (i = 0; i < sc->num_npcs; i++) {
        xEnt* ent = sc->npcs[i];
        if (ent->chkby && xQuickCullIsects(&qcr, &ent->bound.qcd) && xRayHitsBoundFast(r, &ent->bound)) {
            return TRUE;
        }
    }

    return iRayHitsEnvSpecialFast(r, sc->env);
}
