#ifndef XSCENE_H
#define XSCENE_H

#include "xEnt.h"
#include "xEnv.h"
#include "xMemMgr.h"

typedef struct _xCollis xCollis;
struct xSerial;

typedef xBase*(*xSceneResolvIDCallback)(U32);
typedef char*(*xSceneBase2NameCallback)(xBase*);
typedef char*(*xSceneID2NameCallback)(U32);
typedef xEnt*(*xSceneEntCallback)(xEnt*, xScene*, void*);
typedef void(*xRayCameraCallback)(xEnt*);

typedef struct _xScene
{
    U16 flags;
    U16 num_ents;
    U16 num_trigs;
    U16 num_stats;
    U16 num_dyns;
    U16 num_npcs;
    U16 num_act_ents;
    U16 num_nact_ents;
    F32 gravity;
    F32 drag;
    F32 friction;
    U16 num_ents_allocd;
    U16 num_trigs_allocd;
    U16 num_stats_allocd;
    U16 num_dyns_allocd;
    U16 num_npcs_allocd;
    xEnt** trigs;
    xEnt** stats;
    xEnt** dyns;
    xEnt** npcs;
    xEnt** act_ents;
    xEnt** nact_ents;
    xEnv* env;
    xMemPool mempool;
    xSceneResolvIDCallback resolvID;
    xSceneBase2NameCallback base2Name;
    xSceneID2NameCallback id2Name;
} xScene;

extern xScene* g_xSceneCur;

void xSceneInit(xScene* sc, U16 num_trigs, U16 num_stats, U16 num_dyns, U16 num_npcs);
void xSceneSave(xScene* ent, xSerial* s);
void xSceneLoad(xScene* ent, xSerial* s);
void xSceneSetup(xScene* sc);
void xSceneAddEnt(xScene* sc, xEnt* ent);
xBase* xSceneResolvID(xScene* sc, U32 id);
const char* xSceneID2Name(xScene* sc, U32 id);
void xSceneForAllTriggers(xScene* sc, xSceneEntCallback func, void* data);
void xSceneForAllStatics(xScene* sc, xSceneEntCallback func, void* data);
void xSceneForAllDynamics(xScene* sc, xSceneEntCallback func, void* data);
void xSceneForAllNPCs(xScene* sc, xSceneEntCallback func, void* data);
void xRayHitsScene(xScene* sc, xRay3* r, xCollis* coll);
void xRayHitsCameraScene(xScene* sc, xRay3* r, xRayCameraCallback camera_func);

#endif
