#include "xCommon.h"
#include "xDebug.h"

#include <string.h>

#define NUM_DM_NAMES 14
#define MAX_DEBUG_MODES (NUM_DM_NAMES + 32)

static char dm_names[NUM_DM_NAMES][16] = {
    "",
    "dm_collision",
    "dm_anim",
    "dm_ent",
    "dm_shadow",
    "dm_snd",
    "dm_graph",
    "dm_gui",
    "dm_par",
    "dm_fade",
    "dm_render",
    "dm_mem",
    "dm_vram",
    "dm_pad"
};

U32 gFrameCount;
xDebugModeCallback sDebugModeCallbackFunc[MAX_DEBUG_MODES];
const char* sDebugModeName[MAX_DEBUG_MODES];
U32 gDBfont;

void xprintf(const char*, ...)
{
}

S32 xDebugModeAdd(const char* debugModeName)
{
    S32 i;

    for (i = NUM_DM_NAMES; i < MAX_DEBUG_MODES; i++) {
        if (sDebugModeName[i] && strcmp(sDebugModeName[i], debugModeName) == 0) {
            return -1;
        }
    }

    for (i = NUM_DM_NAMES; i < MAX_DEBUG_MODES; i++) {
        if (!sDebugModeName[i]) {
            sDebugModeName[i] = debugModeName;
            return i;
        }
    }

    return -1;
}

S32 xDebugModeAdd(const char* debugModeName, xDebugModeCallback func)
{
    S32 modeid = xDebugModeAdd(debugModeName);

    if (modeid >= 0) {
        xDebugModeRegisterCallback(modeid, func);
    }

    return modeid;
}

void xDebugModeRegisterCallback(S32 mode, xDebugModeCallback func)
{
    sDebugModeCallbackFunc[mode] = func;
}

void xDebugInit()
{
    for (S32 i = 0; i < MAX_DEBUG_MODES; i++) {
        sDebugModeCallbackFunc[i] = NULL;

        if (i < NUM_DM_NAMES) {
            sDebugModeName[i] = dm_names[i];
        } else {
            sDebugModeName[i] = NULL;
        }
    }
}

void xDebugUpdate()
{
}

void xDebugExit()
{
}

void xDebugTimestampScreen()
{
}
