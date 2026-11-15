#ifndef XCOLOR_H
#define XCOLOR_H

#include "iColor.h"

typedef iColor xColor;

inline xColor xColorFromRGBA(U8 r, U8 g, U8 b, U8 a)
{
    xColor c;
    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
    return c;
}

inline void xColorCopy(xColor* o, xColor c)
{
    *(U32*)o = *(U32*)&c;
}

#endif
