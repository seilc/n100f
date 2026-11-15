#ifndef XMATH3_H
#define XMATH3_H

#include "iMath3.h"

typedef struct _xVec3
{
    F32 x, y, z;
} xVec3;

typedef struct _xVec4
{
    F32 x, y, z, w;
} xVec4;

typedef struct _xMat4x3
{
    xVec3 right;
    S32 flags;
    xVec3 up;
    U32 pad1;
    xVec3 at;
    U32 pad2;
    xVec3 pos;
    U32 pad3;
} xMat4x3;

typedef struct _xQuat
{
    xVec3 v;
    F32 s;
} xQuat;

typedef struct _xSphere
{
    xVec3 center;
    F32 r;
} xSphere;

typedef struct _xBox
{
    xVec3 upper;
    xVec3 lower;
} xBox;

typedef struct _xBBox
{
    xVec3 center;
    xBox box;
} xBBox;

typedef struct _xCylinder
{
    xVec3 center;
    F32 r;
    F32 h;
} xCylinder;

typedef struct _xLine3
{
    xVec3 p1;
    xVec3 p2;
} xLine3;

// xRay3::flags
#define XRAY3_USE_MIN (1<<10)
#define XRAY3_USE_MAX (1<<11)

typedef struct _xRay3
{
    xVec3 origin;
    xVec3 dir;
    F32 min_t;
    F32 max_t;
    S32 flags;
} xRay3;

inline void xVec3Init(xVec3* v, F32 _x, F32 _y, F32 _z)
{
    v->x = _x, v->y = _y, v->z = _z;
}

inline void xVec3Add(xVec3* o, const xVec3* a, const xVec3* b)
{
    o->x = a->x + b->x, o->y = a->y + b->y, o->z = a->z + b->z;
}

inline void xVec3SMul(xVec3* o, const xVec3* v, F32 s)
{
    o->x = v->x * s, o->y = v->y * s, o->z = v->z * s;
}

#define xLine3FromRay(ln, r)                                                                                                                        \
do {                                                                                                                                                \
    if ((r)->flags & XRAY3_USE_MIN) {                                                                                                               \
        xVec3 delta;                                                                                                                                \
        xVec3SMul(&delta, &(r)->dir, (r)->min_t);                                                                                                   \
        (ln).p1.x = (r)->origin.x + delta.x, (ln).p1.y = (r)->origin.y + delta.y, (ln).p1.z = (r)->origin.z + delta.z;                              \
    } else {                                                                                                                                        \
        (ln).p1.x = (r)->origin.x, (ln).p1.y = (r)->origin.y, (ln).p1.z = (r)->origin.z;                                                            \
    }                                                                                                                                               \
    if ((r)->flags & XRAY3_USE_MAX) {                                                                                                               \
        F32 len;                                                                                                                                    \
        if ((r)->flags & XRAY3_USE_MIN) {                                                                                                           \
            len = (r)->max_t - (r)->min_t;                                                                                                          \
        } else {                                                                                                                                    \
            len = (r)->max_t;                                                                                                                       \
        }                                                                                                                                           \
        (ln).p2.x = (r)->dir.x * len, (ln).p2.y = (r)->dir.y * len, (ln).p2.z = (r)->dir.z * len;                                                   \
    } else {                                                                                                                                        \
        (ln).p2.x = (r)->dir.x, (ln).p2.y = (r)->dir.y, (ln).p2.z = (r)->dir.z;                                                                     \
    }                                                                                                                                               \
    (ln).p2.x = (ln).p1.x + (ln).p2.x, (ln).p2.y = (ln).p1.y + (ln).p2.y, (ln).p2.z = (ln).p1.z + (ln).p2.z;                                        \
} while (0)

#endif
