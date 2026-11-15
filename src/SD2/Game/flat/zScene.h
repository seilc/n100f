#ifndef ZSCENE_H
#define ZSCENE_H

#include "xScene.h"
#include "zEnt.h"

typedef struct _zPortal zPortal;

struct zScene : xScene
{
    U32 sceneID;
    zPortal* pendingPortal;
    union
    {
        U32 num_ents;
        U32 num_base;
    };
    union
    {
        xBase** base;
        zEnt** ents;
    };
    U32 baseCount[43];
    xBase* baseList[43];
    U32 num_bubbles;
    zEnt** bubbles;
};

#endif
