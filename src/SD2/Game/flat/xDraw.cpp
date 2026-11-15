#include "xCommon.h"
#include "xDraw.h"

#include "xColor.h"
#include "xutil.h"
#include "iDraw.h"

static xColor sgDrawColor = { 0xFF, 0xFF, 0xFF, 0xFF };

void xDrawBegin()
{
    iDrawBegin();
}

void xDrawEnd()
{
    iDrawEnd();
}

void xDrawSetColor(xColor color)
{
    xColorCopy(&sgDrawColor, color);
}

void xDrawSetColor(U8 r, U8 g, U8 b, U8 a)
{
    sgDrawColor = xColorFromRGBA(r, g, b, a);
}

void xDrawLine(const xVec3* a, const xVec3* b)
{
    RwV3d verts[2];
    U32 indices[2];
    indices[0] = 0;
    indices[1] = 1;

    verts[0] = *(const RwV3d*)a;
    verts[1] = *(const RwV3d*)b;

    iQueueLineList(sgDrawColor, verts, indices, NULL, ARRAY_LENGTH(verts), ARRAY_LENGTH(indices));
}

void xDrawLine2D(F32 x1, F32 y1, F32 x2, F32 y2)
{
    RwIm2DVertex vtx[2];
    RwReal z = RwIm2DGetNearScreenZ();

    RwIm2DVertexSetScreenX(&vtx[0], x1);
    RwIm2DVertexSetScreenY(&vtx[0], y1);
    RwIm2DVertexSetScreenZ(&vtx[0], z);
    RwIm2DVertexSetIntRGBA(&vtx[0], sgDrawColor.r, sgDrawColor.g, sgDrawColor.b, sgDrawColor.a);

    RwIm2DVertexSetScreenX(&vtx[1], x2);
    RwIm2DVertexSetScreenY(&vtx[1], y2);
    RwIm2DVertexSetScreenZ(&vtx[1], z);
    RwIm2DVertexSetIntRGBA(&vtx[1], sgDrawColor.r, sgDrawColor.g, sgDrawColor.b, sgDrawColor.a);

    RwIm2DRenderLine(vtx, ARRAY_LENGTH(vtx), 0, 1);
}
