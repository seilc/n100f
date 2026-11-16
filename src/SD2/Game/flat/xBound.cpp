#include "xCommon.h"
#include "xBound.h"

#include "xCollide.h"
#include "xMath3.h"
#include "xQuickCull.h"

xVec3* xBoundCenter(xBound* b)
{
    if (b->type == k_XBOUNDTYPE_SPHERE) {
        return &b->sph.center;
    } else if (b->type == k_XBOUNDTYPE_OBB) {
        return &b->box.center;
    } else if (b->type == k_XBOUNDTYPE_BOX) {
        return &b->box.center;
    } else if (b->type == k_XBOUNDTYPE_CYLINDER) {
        return &b->cyl.center;
    }
    return NULL;
}

void xBoundUpdate(xBound* b)
{
    if (b->type == k_XBOUNDTYPE_BOX) {
        xBoxCenter(&b->box.center, &b->box.box);
    }
    else if (b->type == k_XBOUNDTYPE_OBB) {
        xBoxCenter(&b->box.center, &b->box.box);
        xMat4x3ToWorld(&b->box.center, b->mat, &b->box.center);
    }

    if (b->type != k_XBOUNDTYPE_NONE) {
        xQuickCullForBound(&b->qcd, b);
    }
}

void xBoundHitsBound(const xBound* a, const xBound* b, xCollis* c)
{
    if (!xQuickCullIsects(&a->qcd, &b->qcd)) {
        c->flags &= ~k_HIT_IT;
        return;
    }

    switch (a->type) {
    case k_XBOUNDTYPE_SPHERE:
        switch (b->type) {
        case k_XBOUNDTYPE_SPHERE:
            xSphereHitsSphere(&a->sph, &b->sph, c);
            break;
        case k_XBOUNDTYPE_OBB:
            xSphereHitsOBB(&a->sph, &b->box.box, b->mat, c);
            break;
        case k_XBOUNDTYPE_BOX:
            xSphereHitsBox(&a->sph, &b->box.box, c);
            break;
        }
        break;
    case k_XBOUNDTYPE_BOX:
    case k_XBOUNDTYPE_OBB:
    case k_XBOUNDTYPE_CYLINDER:
        break;
    }
}

void xBoundHitsModel(const xBound* b, const xModelInstance* m, xCollis* colls, U8 ncolls, F32 sth)
{
    switch (b->type) {
    case k_XBOUNDTYPE_SPHERE:
        iSphereHitsModel3(&b->sph, m, colls, ncolls, sth);
        break;
    case k_XBOUNDTYPE_BOX:
    case k_XBOUNDTYPE_OBB:
    case k_XBOUNDTYPE_CYLINDER:
        break;
    }
}

void xRayHitsBound(const xRay3* r, const xBound* b, xCollis* c)
{
    xIsect isect;

    if (b->type == k_XBOUNDTYPE_SPHERE) {
        iSphereIsectRay(&b->sph, r, &isect);
    } else if (b->type == k_XBOUNDTYPE_OBB) {
        xRay3 lr;
        xMat3x3 mn;
        xMat3x3* dmp;
        F32 one_ms = xVec3Length2(&b->mat->right);

        if (xabs(one_ms - 1.0f) >= 0.001f) {
            one_ms = 1.0f / xsqrt(one_ms);
            lr.max_t = r->max_t * one_ms;
            lr.min_t = r->min_t * one_ms;
            xMat3x3Normalize(&mn, (xMat3x3*)b->mat);
            dmp = &mn;
        } else {
            lr.max_t = r->max_t;
            lr.min_t = r->min_t;
            dmp = (xMat3x3*)b->mat;
        }

        xMat4x3ToLocal(&lr.origin, b->mat, &r->origin);
        xMat3x3Tolocal(&lr.dir, dmp, &r->dir);
        lr.flags = r->flags;
        iBoxIsectRay(&b->box.box, &lr, &isect);
    } else if (b->type == k_XBOUNDTYPE_BOX) {
        iBoxIsectRay(&b->box.box, r, &isect);
    }

    if (isect.penned <= 0.0f) {
        c->flags |= k_HIT_IT;
        c->dist = isect.dist;
    } else {
        c->flags &= ~k_HIT_IT;
    }
}

void xRayHitsBound(const xRay3* r, const xQCData* q, const xBound* b, xCollis* c)
{
    if (!xQuickCullIsects(q, &b->qcd)) {
        c->flags &= ~k_HIT_IT;
        return;
    }

    xRayHitsBound(r, b, c);
}

void xVecHitsBound(const xVec3* v, const xBound* b, xCollis* c)
{
    xIsect isect;

    if (b->type == k_XBOUNDTYPE_SPHERE) {
        iSphereIsectVec(&b->sph, v, &isect);
    } else if (b->type == k_XBOUNDTYPE_OBB) {
        xVec3 lv;
        xMat4x3ToLocal(&lv, b->mat, v);
        iBoxIsectVec(&b->box.box, &lv, &isect);
    } else if (b->type == k_XBOUNDTYPE_BOX) {
        iBoxIsectVec(&b->box.box, v, &isect);
    }

    if (isect.penned <= 0.0f) {
        c->flags |= k_HIT_IT;
    } else {
        c->flags &= ~k_HIT_IT;
    }
}
