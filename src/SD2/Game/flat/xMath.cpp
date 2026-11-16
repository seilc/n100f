#include "xCommon.h"
#include "xMath.h"

#include "xMath3.h"

void xMathInit()
{
    iMathInit();
    xMath3Init();
}

void xMathExit()
{
    xMath3Exit();
    iMathExit();
}

U32 xMathSolveQuadratic(F32 a, F32 b, F32 c, F32* x1, F32* x2)
{
    F32 d, dx, p;

    if (a == 0.0f) {
        if (b == 0.0f) {
            return 0;
        }

        *x1 = -c / b;
        return 1;
    }

    d = b * b - 4.0f * a * c;
    if (d < 0.0f) {
        return 0;
    }

    p = 1.0f / (2.0f * a);
    *x1 = -b * p;
    if (d == 0.0f) {
        return 1;
    }

    *x2 = *x1;
    dx = p * xsqrt(d);
    if (a > 0.0f) {
        *x1 -= dx;
        *x2 += dx;
    } else {
        *x1 += dx;
        *x2 -= dx;
    }

    return 2;
}

U32 xMathSolveCubic(F32 a, F32 b, F32 c, F32 d, F32* x1, F32* x2, F32* x3)
{
    F32 arecip, fA, fB, fOffset, fDiscr, fHalfB, fTemp, fDist, fAngle, fCos, fSin;

    if (a == 0.0f) {
        return xMathSolveQuadratic(b, c, d, x1, x2);
    }

    if (a != 1.0f) {
        arecip = 1.0f / a;
        b *= arecip;
        c *= arecip;
        d *= arecip;
    }

    fA = (1.0f / 3.0f) * (3.0f * c - b * b);
    fB = (1.0f / 27.0f) * (2.0f * (b * b * b) - 9.0f * b * c + 27.0f * d);
    fOffset = (1.0f / 3.0f) * b;
    fDiscr = 0.25f * (fB * fB) + (1.0f / 27.0f) * (fA * fA * fA);
    fHalfB = 0.5f * fB;

    if (xabs(fDiscr) < 0.000001f) {
        fDiscr = 0.0f;
    }

    if (fDiscr > 0.0f) {
        fTemp = xsqrt(fDiscr);

        if (-fHalfB + fTemp >= 0.0f) {
            *x1 = xpow(-fHalfB + fTemp, 1.0f / 3.0f);
        } else {
            *x1 = -xpow(-(-fHalfB + fTemp), 1.0f / 3.0f);
        }

        if (-fHalfB - fTemp >= 0.0f) {
            *x1 += xpow(-fHalfB - fTemp, 1.0f / 3.0f);
        } else {
            *x1 -= xpow(-(-fHalfB - fTemp), 1.0f / 3.0f);
        }

        *x1 -= fOffset;
        return 1;
    } else if (fDiscr < 0.0f) {
        fDist = xsqrt(-(1.0f / 3.0f) * fA);
        fAngle = (1.0f / 3.0f) * xatan2(xsqrt(-fDiscr), -fHalfB);
        fCos = icos(fAngle);
        fSin = isin(fAngle);
        *x1 = 2.0f * fDist * fCos - fOffset;
        *x2 = -fDist * (fCos + 1.7320508f * fSin) - fOffset;
        *x3 = -fDist * (fCos - 1.7320508f * fSin) - fOffset;
        return 3;
    } else {
        fTemp = (fHalfB >= 0.0f) ? -xpow(fHalfB, 1.0f / 3.0f) : xpow(-fHalfB, 1.0f / 3.0f);

        *x1 = 2.0f * fTemp - fOffset;
        *x2 = -fTemp - fOffset;
        return 2;
    }
}

F32 xAngleClamp(F32 a)
{
    F32 b = xfmod(a, 2.0f * PI);
    if (b < 0.0f) {
        b += 2.0f * PI;
    }

    return b;
}

F32 xDangleClamp(F32 a)
{
    F32 b = xfmod(a, 2.0f * PI);
    if (b >= PI) {
        b -= 2.0f * PI;
    } else if (b < -PI) {
        b += 2.0f * PI;
    }

    return b;
}
