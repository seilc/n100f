#ifndef IENV_H
#define IENV_H

#include <rwcore.h>
#include <rpworld.h>

struct iEnv
{
    RpWorld* world;
    RpWorld* collision;
    RpWorld* fx;
    RpWorld* camera;
    RpLight* light[2];
    RwFrame* light_frame[2];
    S32 memlvl;
};

#endif
