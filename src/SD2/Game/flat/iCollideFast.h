#ifndef ICOLLIDEFAST_H
#define ICOLLIDEFAST_H

#include "xMath3.h"
#include "xEnv.h"
#include "xModel.h"

typedef struct _xScene xScene;

U32 iRayHitsEnvSpecialFast(const xRay3* r, const xEnv* env);
U32 iRayHitsEnvFast(const xRay3* r, const xEnv* env);
U32 iRayHitsModelFast(const xRay3* r, const xModelInstance* m);
void iCollideFastInit(xScene* sc);

#endif
