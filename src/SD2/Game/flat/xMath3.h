#ifndef XMATH3_H
#define XMATH3_H

#include "iMath3.h"
#include "xMath.h"

#include <string.h>

typedef struct _xVec3
{
    F32 x, y, z;
} xVec3;

typedef struct _xVec4
{
    F32 x, y, z, w;
} xVec4;

typedef struct _xMat3x3
{
    xVec3 right;
    S32 flags;
    xVec3 up;
    U32 pad1;
    xVec3 at;
    U32 pad2;
} xMat3x3;

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

typedef struct _xRot
{
    xVec3 axis;
    F32 angle;
} xRot;

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

typedef struct _xIsect
{
    U32 flags;
    F32 penned;
    F32 contained;
    F32 lapped;
    xVec3 point;
    xVec3 norm;
    F32 dist;
} xIsect;

inline void xVec3Init(xVec3* v, F32 _x, F32 _y, F32 _z)
{
    v->x = _x, v->y = _y, v->z = _z;
}

inline void xVec3Copy(xVec3* o, const xVec3* v)
{
    o->x = v->x, o->y = v->y, o->z = v->z;
}

inline void xVec3Add(xVec3* o, const xVec3* a, const xVec3* b)
{
    o->x = a->x + b->x, o->y = a->y + b->y, o->z = a->z + b->z;
}

inline void xVec3Sub(xVec3* o, const xVec3* a, const xVec3* b)
{
    o->x = a->x - b->x, o->y = a->y - b->y, o->z = a->z - b->z;
}

inline void xVec3SMul(xVec3* o, const xVec3* v, F32 s)
{
    o->x = v->x * s, o->y = v->y * s, o->z = v->z * s;
}

inline void xVec3AddTo(xVec3* a, const xVec3* b)
{
    a->x += b->x, a->y += b->y, a->z += b->z;
}

inline void xVec3SMulBy(xVec3* v, F32 s)
{
    v->x *= s, v->y *= s, v->z *= s;
}

inline F32 xVec3Dot(const xVec3* a, const xVec3* b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

inline F32 xVec3Length(const xVec3* v)
{
    return xsqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

inline F32 xVec3Length2(const xVec3* v)
{
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

#define xBoxCenter(o, b)                        \
do {                                            \
    xVec3Add((o), &(b)->lower, &(b)->upper);    \
    xVec3SMul((o), (o), 0.5f);                  \
} while (0)

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

F32 xVec3Normalize(xVec3* o, const xVec3* v);

inline F32 xVec3Hdng(xVec3* hdng, const xVec3* a, const xVec3* b)
{
    xVec3Sub(hdng, b, a);
    return xVec3Normalize(hdng, hdng);
}

extern const xVec3 g_O3;
extern const xMat4x3 g_I3;

void xMath3Init();
void xMath3Exit();
void xMat3x3Normalize(xMat3x3* o, const xMat3x3* m);
void xMat3x3GetEuler(xMat3x3* m, xVec3* a);
void xMat3x3Mul(xMat3x3* o, const xMat3x3* a, const xMat3x3* b);
void xMat3x3Tolocal(xVec3* o, const xMat3x3* m, const xVec3* v);
void xMat3x3Euler(xMat3x3* m, F32 yaw, F32 pitch, F32 roll);
void xMat3x3LMulVec(xVec3* o, const xMat3x3* m, const xVec3* v);

inline void xMat3x3Copy(xMat3x3* o, const xMat3x3* m)
{
    memcpy(o, m, sizeof(xMat3x3));
}

inline void xMat3x3Identity(xMat3x3* m)
{
    xMat3x3Copy(m, (const xMat3x3*)&g_I3);
}

void xMat3x3RotY(xMat3x3* m, F32 t);
void xMat3x3RMulVec(xVec3* o, const xMat3x3* m, const xVec3* v);

inline void xMat4x3ToWorld(xVec3* o, const xMat4x3* m, const xVec3* v)
{
    xMat3x3RMulVec(o, (const xMat3x3*)m, v);
    o->x = o->x + m->pos.x, o->y = o->y + m->pos.y, o->z = o->z + m->pos.z;
}

inline void xMat4x3ToLocal(xVec3* o, const xMat4x3* m, const xVec3* v)
{
    o->x = v->x - m->pos.x, o->y = v->y - m->pos.y, o->z = v->z - m->pos.z;
    xMat3x3Tolocal(o, (const xMat3x3*)m, o);
}

#endif
