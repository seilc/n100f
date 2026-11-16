#ifndef XDEBUG_H
#define XDEBUG_H

#include "types.h"

typedef void(*xDebugModeCallback)();

void xprintf(const char* fmt, ...);
void xDebugModeService(); // UNUSED
void xDebugInit();
void xDebugUpdate();
void xDebugExit();
void xDebugTimestampScreen();
S32 xDebugModeAdd(const char* debugModeName);
S32 xDebugModeAdd(const char* debugModeName, xDebugModeCallback func);
void xDebugModeRegisterCallback(S32 mode, xDebugModeCallback func);

extern U32 gFrameCount;

#endif
