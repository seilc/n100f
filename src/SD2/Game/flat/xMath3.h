#ifndef XMATH3_H
#define XMATH3_H

typedef struct _xVec3
{
    F32 x, y, z;
} xVec3;

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
