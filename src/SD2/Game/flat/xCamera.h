#ifndef XCAMERA_H
#define XCAMERA_H

#include "xBase.h"
#include "xMath3.h"

#include <rwcore.h>

struct xCamAsset;
typedef struct _xEnv xEnv;

typedef enum _tagCamType
{
    eCamType_Follow,
    eCamType_FollowWithOffset,
    eCamType_Static,
    eCamType_Path,
    eCamType_StaticFollow,
    eCamType_Shoulder,
    eCamType_Total
} eCamType;

typedef enum _tagTransType
{
    eTransType_None,
    eTransType_Interp1,
    eTransType_Interp2,
    eTransType_Interp3,
    eTransType_Interp4,
    eTransType_Linear,
    eTransType_Interp1Rev,
    eTransType_Interp2Rev,
    eTransType_Interp3Rev,
    eTransType_Interp4Rev,
    eTransType_Total
} eTransType;

typedef struct _tagxCamFollow
{
    F32 rotation;
    F32 distance;
    F32 height;
    F32 rubber_band;
    F32 start_speed;
    F32 end_speed;
} xCamFollow;

typedef struct _tagxCamStatic
{
    U32 unused;
} xCamStatic;

typedef struct _tagxCamShoulder
{
    F32 distance;
    F32 height;
    F32 realign_speed;
    F32 realign_delay;
    S32 state;
} xCamShoulder;

typedef struct _tagxCamPath
{
    U32 assetID;
    F32 time_end;
} xCamPath;

typedef struct _tagxCamStaticFollow
{
    F32 rubber_band;
} xCamStaticFollow;

typedef struct _tagCamInfo
{
    union
    {
        xCamFollow f;
        xCamStatic s;
        xCamShoulder sh;
        xCamPath p;
        xCamStaticFollow sf;
    };
} CamInfo;

typedef struct _tagxCam : xBase
{
    U32 valid_flags;
    eCamType cam_type;
    U32 flags;
    xMat4x3 mat;
    xQuat rot;
    F32 fov;
    F32 trans_time;
    eTransType trans_type;
    xVec3 view_offset;
    CamInfo u;
    xCamAsset* asset;
} xCam;

typedef struct _tagxCamFrame
{
    RwCamera* lo_cam;
    xCam orig_cam;
    xCam hi_cam;
    xVec4 frustplane[8];
    xCam* t_cam;
    F32 t_time;
    F32 g_time;
    xEnv* env;
    xVec3* target_pos;
    xMat4x3* target_frame;
    xVec3 pre_align_dir;
    xCam** cameras;
    U32 total_cameras;
} xCamFrame;

#endif
