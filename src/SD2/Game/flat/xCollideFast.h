#ifndef XCOLLIDEFAST_H
#define XCOLLIDEFAST_H

#include "xMath3.h"

typedef struct _xBound xBound;
typedef struct _xEnv xEnv;
typedef struct _xScene xScene;

void xCollideFastInit(xScene* sc);
U32 xRayHitsSphereFast(const xRay3* r, const xSphere* s);
U32 xRayHitsBoxFast(const xRay3* r, const xBox* b);
U32 xRayHitsCylinderFast(const xRay3* r, const xCylinder* c);
U32 xRayHitsBoundFast(const xRay3* r, const xBound* b);
U32 xRayHitsEnvFast(const xRay3* r, const xEnv* env);
U32 xRayHitsSceneFast(const xRay3* r, xScene* sc);

#endif
