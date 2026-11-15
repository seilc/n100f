#ifndef XDRAW_H
#define XDRAW_H

#include "xColor.h"
#include "xMath3.h"

void xDrawBegin();
void xDrawEnd();
void xDrawSetColor(xColor color);
void xDrawSetColor(U8 r, U8 g, U8 b, U8 a);
void xDrawLine(const xVec3* a, const xVec3* b);
void xDrawLine2D(F32 x1, F32 y1, F32 x2, F32 y2);

#endif
