#ifndef IMATH3_H
#define IMATH3_H

typedef struct _xBox xBox;
typedef struct _xSphere xSphere;
typedef struct _xMat4x3 xMat4x3;

void iSphereInitBoundOBB(xSphere* s, const xBox* b, const xMat4x3* m);

#endif
