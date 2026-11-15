#ifndef XQUICKCULL_H
#define XQUICKCULL_H

#include "xMath3.h"

struct xQCData
{
    U32 xmask;
    U32 ymask;
    U32 zmask;
};

typedef struct _xBound xBound;

void xQuickCullInit(F32 xmin, F32 ymin, F32 zmin, F32 xmax, F32 ymax, F32 zmax);
void xQuickCullInit(const xBox* box);
S32 xQuickCullIsects(const xQCData* a, const xQCData* b);
void xQuickCullForBound(xQCData* q, const xBound* b);
void xQuickCullForRay(xQCData* q, const xRay3* r);
void xQuickCullForSphere(xQCData* q, const xSphere* s);
void xQuickCullForBox(xQCData* q, const xBox* box);
void xQuickCullForOBB(xQCData* q, const xBox* b, const xMat4x3* m);

#endif
