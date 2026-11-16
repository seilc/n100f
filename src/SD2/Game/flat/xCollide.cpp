#include "xCommon.h"
#include "xCollide.h"

xCollsIdx xCollideGetCollsIdx(const xVec3* tohit, const xMat3x3* mat)
{
    if (xsqr(tohit->y) > xsqr(tohit->x) + xsqr(tohit->z)) {
        if (tohit->y < 0.0f) {
            return k_XCOLLS_IDX_FLOOR;
        }

        return k_XCOLLS_IDX_CEIL;
    }

    F32 local_x = mat->right.x * tohit->x + mat->right.z * tohit->z;
    F32 local_z = mat->at.x * tohit->x + mat->at.z * tohit->z;

    if (local_x > 0.0f) {
        if (local_z > 0.0f) {
            if (local_x > local_z) {
                return k_XCOLLS_IDX_LEFT;
            }

            return k_XCOLLS_IDX_FRONT;
        }

        if (local_x > -local_z) {
            return k_XCOLLS_IDX_LEFT;
        }

        return k_XCOLLS_IDX_REAR;
    }

    if (local_z > 0.0f) {
        if (local_x < -local_z) {
            return k_XCOLLS_IDX_RIGHT;
        }

        return k_XCOLLS_IDX_FRONT;
    }

    if (local_x < local_z) {
        return k_XCOLLS_IDX_RIGHT;
    }

    return k_XCOLLS_IDX_REAR;
}

void xCollideInit(xScene* sc)
{
    iCollideInit(sc);
}

U32 xSphereHitsSphere(const xSphere* a, const xSphere* b, xCollis* coll)
{
    xIsect isx;
    iSphereIsectSphere(b, a, &isx);

    if (!(isx.penned <= 0.0f)) {
        coll->flags &= ~k_HIT_IT;
        return FALSE;
    }

    if (isx.contained <= 0.0f) {
        coll->flags |= 0x10;
    }

    coll->dist = a->r + isx.penned;

    if (coll->flags & 0x1600) {
        if (isx.dist == 0.0f) {
            xVec3Copy(&coll->tohit, &g_O3);
        } else {
            F32 scale = -coll->dist / isx.dist;
            xVec3SMul(&coll->tohit, &isx.norm, scale);
        }
    }

    if (coll->flags & 0x800) {
        if (isx.dist == 0.0f) {
            xVec3Copy(&coll->depen, &g_O3);
        } else {
            F32 scale = -isx.penned / isx.dist;
            xVec3SMul(&coll->depen, &isx.norm, scale);
        }
    }

    if (coll->flags & 0x1200) {
        xVec3Normalize(&coll->hdng, &coll->tohit);
    }

    if (coll->flags & 0x200) {
        xVec3SMul(&coll->norm, &coll->hdng, -1.0f);
    }

    coll->flags |= k_HIT_IT;
    return TRUE;
}

U32 xSphereHitsBox(const xSphere* a, const xBox* b, xCollis* coll)
{
    xIsect isx;
    iBoxIsectSphere(b, a, &isx);

    if (!(isx.penned <= 0.0f)) {
        coll->flags &= ~k_HIT_IT;
        return FALSE;
    }

    if (isx.contained <= 0.0f) {
        coll->flags |= 0x10;
    }

    coll->dist = isx.dist;

    if (coll->flags & 0x400) {
        xVec3Copy(&coll->tohit, &isx.norm);
    }

    if (coll->flags & 0x800) {
        if (isx.dist == 0.0f) {
            xVec3Copy(&coll->depen, &g_O3);
        } else {
            F32 scale = isx.penned / isx.dist;
            xVec3SMul(&coll->depen, &isx.norm, scale);
        }
    }

    if (coll->flags & 0x1200) {
        xVec3Normalize(&coll->hdng, &isx.norm);
    }

    if (coll->flags & 0x200) {
        xVec3SMul(&coll->norm, &coll->hdng, -1.0f);
    }

    coll->flags |= k_HIT_IT;
    return TRUE;
}

U32 xSphereHitsOBB(const xSphere* a, const xBox* b, const xMat4x3* m, xCollis* coll)
{
    xSphere mxa;
    F32 mscale = xVec3Length(&m->right);
    xMat3x3 mnormal;
    xMat3x3Normalize(&mnormal, (const xMat3x3*)m);
    xMat4x3ToLocal(&mxa.center, m, &a->center);
    mxa.r = a->r / mscale;
    xSphereHitsBox(&mxa, b, coll);
    if (!(coll->flags & k_HIT_IT)) {
        return FALSE;
    }

    if (coll->flags & 0x200) {
        xMat3x3RMulVec(&coll->norm, &mnormal, &coll->norm);
    }

    if (coll->flags & 0x800) {
        xMat3x3RMulVec(&coll->depen, (const xMat3x3*)m, &coll->depen);
    }

    if (coll->flags & 0x400) {
        xMat3x3RMulVec(&coll->tohit, (const xMat3x3*)m, &coll->tohit);
    }

    if (coll->flags & k_HIT_CALC_HDNG) {
        xMat3x3RMulVec(&coll->hdng, &mnormal, &coll->hdng);
    }

    coll->dist *= mscale;
    return TRUE;
}
