#ifndef IMATH_H
#define IMATH_H

#include "types.h"

#include <math.h>

#define iabs(x) (float)(__fabs((float)(x)))

void iMathInit();
void iMathExit();
U32 irand();
F32 iurand();

inline F32 isin(F32 f)
{
    return sinf(f);
}

inline F32 icos(F32 f)
{
    return cosf(f);
}

#endif
