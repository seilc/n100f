#ifndef XMATH_H
#define XMATH_H

#include "iMath.h"

#include <math.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define CLAMP(x,a,b) MAX((a), MIN((x), (b)))

F32 xsqrt(F32 x);
F32 xexp(F32 x);

#include "xMathInlines.h"

#endif
