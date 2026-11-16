#ifndef XMATH_H
#define XMATH_H

#include "iMath.h"

#include <math.h>

#define PI 3.14159265358979323846f

#define xabs(x) iabs(x)
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define CLAMP(x,a,b) MAX((a), MIN((x), (b)))

void xMathInit();
void xMathExit();
U32 xMathSolveQuadratic(F32 a, F32 b, F32 c, F32* x1, F32* x2);
U32 xMathSolveCubic(F32 a, F32 b, F32 c, F32 d, F32* x1, F32* x2, F32* x3);
F32 xAngleClamp(F32 a);
F32 xDangleClamp(F32 a);

F32 xsqrt(F32 x);
F32 xexp(F32 x);
F32 xpow(F32 x, F32 y);
F32 xatan2(F32 y, F32 x);
F32 xfmod(F32 x, F32 y);

#include "xMathInlines.h"

#endif
