#ifndef RWCORE_H
#define RWCORE_H

#include "rwplcore.h"

typedef struct RwBBox RwBBox;
typedef struct RwCamera RwCamera;
typedef struct RwFrame RwFrame;
typedef struct RwRaster RwRaster;
typedef struct RxObjSpace3DVertex RxObjSpace3DVertex;

typedef RxObjSpace3DVertex RxObjSpace3DLitVertex;
typedef RxObjSpace3DLitVertex RwIm3DVertex;

enum RwIm3DTransformFlags
{
    rwIM3D_VERTEXUV = 0x1,
    rwIM3D_ALLOPAQUE = 0x2,
    rwIM3D_NOCLIP = 0x4,
    rwIM3D_VERTEXXYZ = 0x8,
    rwIM3D_VERTEXRGBA = 0x10,
    rwIM3DTRANSFORMFLAGSFORCEENUMSIZEINT = RWFORCEENUMSIZEINT,
};
typedef enum RwIm3DTransformFlags RwIm3DTransformFlags;

struct RxObjSpace3DVertex
{
    RwV3d objVertex;
    RwV3d objNormal;
    RwUInt32 color;
    RwReal u;
    RwReal v;
};

struct RwBBox
{
    RwV3d sup;
    RwV3d inf;
};

struct RwFrame
{
    RwObject object;
    RwLLLink inDirtyListLink;
    RwMatrix modelling;
    RwMatrix ltm;
    RwLinkList objectList;
    RwFrame* child;
    RwFrame* next;
    RwFrame* root;
};

#define RxObjSpace3DLitVertexSetU(_vert, _imu) ((_vert)->u = (_imu))
#define RxObjSpace3DLitVertexSetV(_vert, _imv) ((_vert)->v = (_imv))

#define RwIm3DVertexSetU RxObjSpace3DLitVertexSetU
#define RwIm3DVertexSetV RxObjSpace3DLitVertexSetV

#define RwIm3DVertexGetPos(_vert) (&((_vert)->objVertex))

#define RwIm3DVertexSetPos(_vert, _imx, _imy, _imz)  \
    MACRO_START                                      \
    {                                                \
        (_vert)->objVertex.x = _imx;                 \
        (_vert)->objVertex.y = _imy;                 \
        (_vert)->objVertex.z = _imz;                 \
    }                                                \
    MACRO_STOP

#define RwIm3DVertexSetRGBA(_vert, _r, _g, _b, _a)  \
    MACRO_START                                     \
    {                                               \
        ((_vert)->color = ((_r) << 24) |            \
                          ((_g) << 16) |            \
                          ((_b) << 8) |             \
                          ((_a)));                  \
    }                                               \
    MACRO_STOP

#define RwCameraGetCurrentCameraMacro() ((RwCamera *)RWSRCGLOBAL(curCamera))
#define RwCameraGetFrameMacro(_camera) ((RwFrame *)rwObjectGetParent((_camera)))
#define RwFrameGetMatrixMacro(_f) (&(_f)->modelling)

#ifdef __cplusplus
extern "C" {
#endif

RwBool RwIm3DEnd(void);
RwBool RwIm3DRenderPrimitive(RwPrimitiveType primType);
void* RwIm3DTransform(RwIm3DVertex* pVerts, RwUInt32 numVerts, RwMatrix* ltm, RwUInt32 flags);

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
RwCamera* RwCameraGetCurrentCamera(void);
RwFrame* RwCameraGetFrame(const RwCamera* camera);
RwMatrix* RwFrameGetMatrix(RwFrame* frame);
#endif

#ifdef __cplusplus
}
#endif

#if !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
#define RwCameraGetCurrentCamera() RwCameraGetCurrentCameraMacro()
#define RwCameraGetFrame(_camera) RwCameraGetFrameMacro(_camera)
#define RwFrameGetMatrix(_f) RwFrameGetMatrixMacro(_f)
#endif

#endif
