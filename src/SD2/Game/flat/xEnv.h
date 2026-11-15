#ifndef XENV_H
#define XENV_H

#include "iEnv.h"

typedef struct _xEnv
{
    iEnv* geom;
    iEnv ienv;
} xEnv;

extern xEnv* gCurXEnv;

void xEnvLoadBsp(xEnv* env, const void* data, U32 datasize, S32 dataType);
void xEnvSetup(xEnv* env);
void xEnvFree(xEnv* env);
void xEnvRender(xEnv* env);

#endif
