#ifndef IMATH_H
#define IMATH_H

#include "types.h"

#include <math.h>

U32 irand();
F32 iurand();

inline F32 isin(F32 f)
{
    return sinf(f);
}

#endif
