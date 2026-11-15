#include "xCommon.h"
#include "iDraw.h"

#include "xMath3.h"

#define MAX_VERTS 64

void iDrawSetFBMSK(U32 FBMSK)
{
    if (FBMSK == 0xFFFFFFFF) {
        GXSetColorUpdate(GX_FALSE);
        GXSetAlphaUpdate(GX_FALSE);
    } else if (FBMSK == 0) {
        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaUpdate(GX_TRUE);
    }
}

void iDrawBillboard(xVec3* pos, U32 count, F32 dx, F32 dy, RwRaster* raster)
{
    U32 i;
    RwIm3DVertex vert[MAX_VERTS * 6];
    RwIm3DVertex* vp;
    xVec3 vdx, vdy;
    RwMatrix* cammat;
    xVec3* rt;
    xVec3* up;
    xVec3* at;
    xVec3 to;

    cammat = RwFrameGetMatrix(RwCameraGetFrame(RwCameraGetCurrentCamera()));

    if (!pos || !cammat || !raster) {
        return;
    }

    rt = (xVec3*)&cammat->right;
    up = (xVec3*)&cammat->up;
    at = (xVec3*)&cammat->at;

    xVec3SMul(&vdx, rt, 0.5f * dx);
    xVec3SMul(&vdy, up, -0.5f * dy);
    xVec3SMul(&to, at, -0.25f);

    vp = vert;
    for (i = 0; i < count; i++) {
        RwIm3DVertexSetPos(vp, pos->x - vdx.x - vdy.x, pos->y - vdx.y - vdy.y, pos->z - vdx.z - vdy.z);
        RwIm3DVertexSetRGBA(vp, 0x80, 0x80, 0x80, 0x80);
        RwIm3DVertexSetU(vp, 0.0f);
        RwIm3DVertexSetV(vp, 0.0f);
        RwV3dAdd(RwIm3DVertexGetPos(vp), RwIm3DVertexGetPos(vp), &to);
        vp++;

        RwIm3DVertexSetPos(vp, pos->x + vdx.x - vdy.x, pos->y + vdx.y - vdy.y, pos->z + vdx.z - vdy.z);
        RwIm3DVertexSetRGBA(vp, 0x80, 0x80, 0x80, 0x80);
        RwIm3DVertexSetU(vp, 0.999f);
        RwIm3DVertexSetV(vp, 0.0f);
        RwV3dAdd(RwIm3DVertexGetPos(vp), RwIm3DVertexGetPos(vp), &to);
        vp++;

        RwIm3DVertexSetPos(vp, pos->x - vdx.x + vdy.x, pos->y - vdx.y + vdy.y, pos->z - vdx.z + vdy.z);
        RwIm3DVertexSetRGBA(vp, 0x80, 0x80, 0x80, 0x80);
        RwIm3DVertexSetU(vp, 0.0f);
        RwIm3DVertexSetV(vp, 0.999f);
        RwV3dAdd(RwIm3DVertexGetPos(vp), RwIm3DVertexGetPos(vp), &to);
        vp++;

        RwIm3DVertexSetPos(vp, pos->x - vdx.x + vdy.x, pos->y - vdx.y + vdy.y, pos->z - vdx.z + vdy.z);
        RwIm3DVertexSetRGBA(vp, 0x80, 0x80, 0x80, 0x80);
        RwIm3DVertexSetU(vp, 0.0f);
        RwIm3DVertexSetV(vp, 0.999f);
        RwV3dAdd(RwIm3DVertexGetPos(vp), RwIm3DVertexGetPos(vp), &to);
        vp++;

        RwIm3DVertexSetPos(vp, pos->x + vdx.x - vdy.x, pos->y + vdx.y - vdy.y, pos->z + vdx.z - vdy.z);
        RwIm3DVertexSetRGBA(vp, 0x80, 0x80, 0x80, 0x80);
        RwIm3DVertexSetU(vp, 0.999f);
        RwIm3DVertexSetV(vp, 0.0f);
        RwV3dAdd(RwIm3DVertexGetPos(vp), RwIm3DVertexGetPos(vp), &to);
        vp++;

        RwIm3DVertexSetPos(vp, pos->x + vdx.x + vdy.x, pos->y + vdx.y + vdy.y, pos->z + vdx.z + vdy.z);
        RwIm3DVertexSetRGBA(vp, 0x80, 0x80, 0x80, 0x80);
        RwIm3DVertexSetU(vp, 0.999f);
        RwIm3DVertexSetV(vp, 0.999f);
        RwV3dAdd(RwIm3DVertexGetPos(vp), RwIm3DVertexGetPos(vp), &to);
        vp++;

        pos++;
    }

    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*)raster);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)FALSE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)FALSE);

    RwIm3DTransform(vert, count * 6, NULL, rwIM3D_VERTEXUV | rwIM3D_VERTEXXYZ);
    RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
    RwIm3DEnd();

    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
}

void iDrawBegin() {}

void iQueueLineList(iColor, RwV3d*, U32*, RwMatrix*, U32, U32) {}

void iDrawEnd() {}
