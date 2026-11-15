#ifndef IDRAW_H
#define IDRAW_H

#include "iColor.h"

#include <rwcore.h>

typedef struct _xVec3 xVec3;

void iDrawBegin();
void iQueueLineList(iColor color, RwV3d* verts, U32* indices, RwMatrix* mat, U32 numVerts, U32 numIndices);
void iDrawEnd();
void iDrawSetFBMSK(U32 FBMSK);
void iDrawBillboard(xVec3* pos, U32 count, F32 dx, F32 dy, RwRaster* raster);

#endif
