#include "xCommon.h"
#include "xQuickCull.h"

#include "xBound.h"
#include "xMath.h"

static F32 world_xmin;
static F32 world_ymin;
static F32 world_zmin;
static F32 world_xmax;
static F32 world_ymax;
static F32 world_zmax;
static F32 world_xsz;
static F32 world_ysz;
static F32 world_zsz;
static F32 block_xsz;
static F32 block_ysz;
static F32 block_zsz;
static F32 one_block_xsz;
static F32 one_block_ysz;
static F32 one_block_zsz;
static F32 lcl_world_xsz;
static F32 lcl_world_ysz;
static F32 lcl_world_zsz;
static F32 ucl_world_xsz;
static F32 ucl_world_ysz;
static F32 ucl_world_zsz;

void xQuickCullInit(F32 xmin, F32 ymin, F32 zmin, F32 xmax, F32 ymax, F32 zmax)
{
    world_xmin = xmin;
    world_ymin = ymin;
    world_zmin = zmin;
    world_xmax = xmax;
    world_ymax = ymax;
    world_zmax = zmax;
    world_xsz = xmax - xmin;
    world_ysz = ymax - ymin;
    world_zsz = zmax - zmin;

    if (world_xsz <= 0.0f || world_ysz <= 0.0f || world_zsz <= 0.0f) {
        world_xsz = world_ysz = world_zsz = 1.0f;
    }

    lcl_world_xsz = xmin - world_xsz;
    lcl_world_ysz = ymin - world_ysz;
    lcl_world_zsz = zmin - world_zsz;
    block_xsz = world_xsz / 30.0f;
    block_ysz = world_ysz / 30.0f;
    block_zsz = world_zsz / 30.0f;
    one_block_xsz = 1.0f / block_xsz;
    one_block_ysz = 1.0f / block_ysz;
    one_block_zsz = 1.0f / block_zsz;
    ucl_world_xsz = xmax + world_xsz;
    ucl_world_ysz = ymax + world_ysz;
    ucl_world_zsz = zmax + world_zsz;
}

void xQuickCullInit(const xBox* box)
{
    xQuickCullInit(box->lower.x, box->lower.y, box->lower.z,
                   box->upper.x, box->upper.y, box->upper.z);
}

S32 xQuickCullIsects(const xQCData* a, const xQCData* b)
{
    // BUG: Should be &&
    return (a->xmask & b->xmask) | (a->ymask & b->ymask) | (a->zmask & b->zmask);
}

void xQuickCullForBound(xQCData* q, const xBound* b)
{
    if (b->type == k_XBOUNDTYPE_SPHERE) {
        xQuickCullForSphere(q, &b->sph);
    } else if (b->type == k_XBOUNDTYPE_BOX) {
        xQuickCullForBox(q, &b->box.box);
    } else if (b->type == k_XBOUNDTYPE_OBB) {
        xQuickCullForOBB(q, &b->box.box, b->mat);
    }
}

static void xQuickCullCellForVec(xQCData* c, const xVec3* v)
{
    F32 fx = CLAMP(v->x, lcl_world_xsz, ucl_world_xsz);
    F32 fy = CLAMP(v->y, lcl_world_ysz, ucl_world_ysz);
    F32 fz = CLAMP(v->z, lcl_world_zsz, ucl_world_zsz);

    S16 x = (F32)floor((fx - world_xmin) / block_xsz);
    S16 y = (F32)floor((fy - world_ymin) / block_ysz);
    S16 z = (F32)floor((fz - world_zmin) / block_zsz);

    if (x < 0) {
        c->xmask = 0;
    } else if (x > 29) {
        c->xmask = 31;
    } else {
        c->xmask = x + 1;
    }

    if (y < 0) {
        c->ymask = 0;
    } else if (y > 29) {
        c->ymask = 31;
    } else {
        c->ymask = y + 1;
    }

    if (z < 0) {
        c->zmask = 0;
    } else if (z > 29) {
        c->zmask = 31;
    } else {
        c->zmask = z + 1;
    }
}

static void xQuickCullCellMerge(xQCData* o, const xQCData* a, const xQCData* b)
{
    S16 xmin = MIN(a->xmask, b->xmask);
    S16 ymin = MIN(a->ymask, b->ymask);
    S16 zmin = MIN(a->zmask, b->zmask);
    S16 xmax = MAX(a->xmask, b->xmask);
    S16 ymax = MAX(a->ymask, b->ymask);
    S16 zmax = MAX(a->zmask, b->zmask);

    if (xmax == 31 && xmin == 0) {
        o->xmask = 0xFFFFFFFF;
    } else {
        o->xmask = ((1 << (xmax - xmin + 1)) - 1) << xmin;
    }

    if (ymax == 31 && ymin == 0) {
        o->ymask = 0xFFFFFFFF;
    } else {
        o->ymask = ((1 << (ymax - ymin + 1)) - 1) << ymin;
    }

    if (zmax == 31 && zmin == 0) {
        o->zmask = 0xFFFFFFFF;
    } else {
        o->zmask = ((1 << (zmax - zmin + 1)) - 1) << zmin;
    }
}

void xQuickCullForRay(xQCData* q, const xRay3* r)
{
    xLine3 ln;

    xLine3FromRay(ln, r);

    xQCData a, b;
    xQuickCullCellForVec(&a, &ln.p1);

    xQuickCullCellForVec(&b, &ln.p2);

    xQuickCullCellMerge(q, &a, &b);
}

void xQuickCullForSphere(xQCData* q, const xSphere* s)
{
    xVec3 v1, v2;

    xVec3Init(&v1, -s->r, -s->r, -s->r);
    xVec3Init(&v2, s->r, s->r, s->r);
    xVec3Add(&v1, &s->center, &v1);
    xVec3Add(&v2, &s->center, &v2);

    xQCData a, b;
    xQuickCullCellForVec(&a, &v1);

    xQuickCullCellForVec(&b, &v2);

    xQuickCullCellMerge(q, &a, &b);
}

void xQuickCullForBox(xQCData* q, const xBox* box)
{
    xQCData a, b;
    xQuickCullCellForVec(&a, &box->lower);

    xQuickCullCellForVec(&b, &box->upper);

    xQuickCullCellMerge(q, &a, &b);
}

void xQuickCullForOBB(xQCData* q, const xBox* b, const xMat4x3* m)
{
    xSphere worldsphere;
    iSphereInitBoundOBB(&worldsphere, b, m);

    xQuickCullForSphere(q, &worldsphere);
}
