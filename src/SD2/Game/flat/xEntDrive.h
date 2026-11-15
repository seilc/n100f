#ifndef XENTDRIVE_H
#define XENTDRIVE_H

#include "xEnt.h"

typedef struct _xEntDrive
{
    U32 flags;
    F32 otm;
    F32 otmr;
    F32 os;
    F32 tm;
    F32 tmr;
    F32 s;
    xEnt* odriver;
    xEnt* driver;
    xEnt* driven;
    xVec3 op;
    xVec3 p;
    xVec3 q;
    F32 yaw;
} xEntDrive;

#endif
