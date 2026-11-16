#ifndef ICOLLIDE_H
#define ICOLLIDE_H

#include "types.h"

typedef struct _xCollis xCollis;
typedef struct _xEnv xEnv;
typedef struct _xRay3 xRay3;
typedef struct _xScene xScene;

typedef struct _iCollis
{
    S32 unknown;
} iCollis;

void iCollideInit(xScene* sc);
U32 iRayHitsEnv(const xRay3* r, const xEnv* env, xCollis* coll);

#endif
