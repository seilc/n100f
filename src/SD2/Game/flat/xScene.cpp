#include "xCommon.h"
#include "xScene.h"

#include "xAnim.h"
#include "xBound.h"
#include "xCollide.h"
#include "xCollideFast.h"
#include "xModel.h"
#include "xQuickCull.h"
#include "iCollide.h"

xScene* g_xSceneCur;

void xSceneInit(xScene* sc, U16 num_trigs, U16 num_stats, U16 num_dyns, U16 num_npcs)
{
    sc->flags = 0;

    sc->num_trigs_allocd = num_trigs;
    sc->trigs = (xEnt**)xMemAlloc(gActiveHeap, (U32)num_trigs * sizeof(xEnt*));

    sc->num_stats_allocd = num_stats;
    sc->stats = (xEnt**)xMemAlloc(gActiveHeap, (U32)num_stats * sizeof(xEnt*));

    sc->num_dyns_allocd = num_dyns;
    sc->dyns = (xEnt**)xMemAlloc(gActiveHeap, (U32)num_dyns * sizeof(xEnt*));

    sc->num_npcs_allocd = num_npcs;
    sc->npcs = (xEnt**)xMemAlloc(gActiveHeap, (U32)num_npcs * sizeof(xEnt*));

    sc->num_ents_allocd = num_trigs + num_stats + num_dyns + num_npcs;
    sc->num_act_ents = 0;
    sc->act_ents = (xEnt**)xMemAlloc(gActiveHeap, sc->num_ents_allocd * sizeof(xEnt*));
    sc->num_nact_ents = 0;
    sc->nact_ents = (xEnt**)xMemAlloc(gActiveHeap, sc->num_ents_allocd * sizeof(xEnt*));

    sc->num_ents = 0;
    sc->num_trigs = 0;
    sc->num_stats = 0;
    sc->num_dyns = 0;
    sc->num_npcs = 0;

    sc->resolvID = NULL;

    xAnimInit();
    xModelInit();

    xAnimPoolInit(&sc->mempool, 50, 1, 1, 4);

    xModelPoolInit(num_npcs + 24, 64);
    xModelPoolInit(num_npcs + 24 + num_dyns, 8);
    xModelPoolInit(num_npcs + 24 + num_dyns + num_stats, 1);

    xAnimTempTransitionInit(16);

    xCollideInit(sc);
    xCollideFastInit(sc);
}

void xSceneSave(xScene*, xSerial*)
{
}

void xSceneLoad(xScene*, xSerial*)
{
}

void xSceneSetup(xScene* sc)
{
    xEnvSetup(sc->env);
}

void xSceneAddEnt(xScene* sc, xEnt* ent)
{
    if (ent->collType == k_XENT_COLLTYPE_TRIG) {
        sc->trigs[sc->num_trigs++] = ent;
    } else if (ent->collType == k_XENT_COLLTYPE_STAT) {
        sc->stats[sc->num_stats++] = ent;
    } else if (ent->collType == k_XENT_COLLTYPE_DYN) {
        sc->dyns[sc->num_dyns++] = ent;
    } else if (ent->collType == k_XENT_COLLTYPE_NPC) {
        sc->npcs[sc->num_npcs++] = ent;
    }

    sc->act_ents[sc->num_act_ents++] = ent;
}

xBase* xSceneResolvID(xScene* sc, U32 id)
{
    if (sc->resolvID) {
        return sc->resolvID(id);
    }

    return NULL;
}

const char* xSceneID2Name(xScene* sc, U32 id)
{
    if (sc->id2Name) {
        return sc->id2Name(id);
    }

    return NULL;
}

void xSceneForAllTriggers(xScene* sc, xSceneEntCallback func, void* data)
{
    for (U16 i = 0; i < sc->num_trigs; i++) {
        if (!func(sc->trigs[i], sc, data)) {
            break;
        }
    }
}

void xSceneForAllStatics(xScene* sc, xSceneEntCallback func, void* data)
{
    for (U16 i = 0; i < sc->num_stats; i++) {
        if (!func(sc->stats[i], sc, data)) {
            break;
        }
    }
}

void xSceneForAllDynamics(xScene* sc, xSceneEntCallback func, void* data)
{
    for (U16 i = 0; i < sc->num_dyns; i++) {
        if (!func(sc->dyns[i], sc, data)) {
            break;
        }
    }
}

void xSceneForAllNPCs(xScene* sc, xSceneEntCallback func, void* data)
{
    for (U16 i = 0; i < sc->num_npcs; i++) {
        if (!func(sc->npcs[i], sc, data)) {
            break;
        }
    }
}

void xRayHitsScene(xScene* sc, xRay3* r, xCollis* coll)
{
    U16 i;
    xCollis c;
    xQCData q;

    xQuickCullForRay(&q, r);

    coll->dist = 1e19f;

    for (i = 0; i < sc->num_stats; i++) {
        xEnt* ent = sc->stats[i];

        xRayHitsBound(r, &q, &ent->bound, &c);

        if ((c.flags & k_HIT_IT) && c.dist < coll->dist) {
            coll->dist = c.dist;
            coll->oid = ent->id;
            coll->optr = ent;
        }
    }

    for (i = 0; i < sc->num_dyns; i++) {
        xEnt* ent = sc->dyns[i];

        xRayHitsBound(r, &q, &ent->bound, &c);

        if ((c.flags & k_HIT_IT) && c.dist < coll->dist) {
            coll->dist = c.dist;
            coll->oid = ent->id;
            coll->optr = ent;
        }
    }

    for (i = 0; i < sc->num_npcs; i++) {
        xEnt* ent = sc->npcs[i];

        xRayHitsBound(r, &q, &ent->bound, &c);

        if ((c.flags & k_HIT_IT) && c.dist < coll->dist) {
            coll->dist = c.dist;
            coll->oid = ent->id;
            coll->optr = ent;
        }
    }

    iRayHitsEnv(r, sc->env, &c);

    if (c.dist < coll->dist) {
        coll->dist = c.dist;
        coll->oid = c.oid;
        coll->optr = NULL;
    }

    if (coll->dist < 1e19f) {
        coll->flags |= k_HIT_IT;
    } else {
        coll->flags &= ~k_HIT_IT;
    }
}

void xRayHitsCameraScene(xScene* sc, xRay3* r, xRayCameraCallback camera_func)
{
    U16 i;
    xQCData qcr;

    xQuickCullForRay(&qcr, r);

    for (i = 0; i < sc->num_stats; i++) {
        xEnt* ent = sc->stats[i];

        if ((ent->moreFlags & k_MORE_FLAGS_CAMERA_COLL) && xQuickCullIsects(&qcr, &ent->bound.qcd) && xRayHitsBoundFast(r, &ent->bound)) {
            camera_func(ent);
        }
    }

    for (i = 0; i < sc->num_dyns; i++) {
        xEnt* ent = sc->dyns[i];

        if ((ent->moreFlags & k_MORE_FLAGS_CAMERA_COLL) && xQuickCullIsects(&qcr, &ent->bound.qcd) && xRayHitsBoundFast(r, &ent->bound)) {
            camera_func(ent);
        }
    }
}
