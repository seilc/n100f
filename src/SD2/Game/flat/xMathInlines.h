#ifndef XMATHINLINES_H
#define XMATHINLINES_H

inline F32 xsqrt(F32 x)
{
    return sqrtf(x);
}

inline F32 xexp(F32 x)
{
    return expf(x);
}

inline F32 xfmod(F32 x, F32 y)
{
    return fmodf(x, y);
}

inline F32 xpow(F32 x, F32 y)
{
    return powf(x, y);
}

inline F32 xatan2(F32 y, F32 x)
{
    return xAngleClamp(atan2f(y, x));
}

#endif
