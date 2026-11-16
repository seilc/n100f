#ifndef IMATH3_H
#define IMATH3_H

typedef struct _xBox xBox;
typedef struct _xSphere xSphere;
typedef struct _xCylinder xCylinder;
typedef struct _xRay3 xRay3;
typedef struct _xIsect xIsect;
typedef struct _xMat4x3 xMat4x3;
typedef struct _xVec3 xVec3;

void iSphereIsectVec(const xSphere* s, const xVec3* v, xIsect* isx);
void iSphereIsectRay(const xSphere* s, const xRay3* r, xIsect* isx);
void iSphereIsectSphere(const xSphere* s, const xSphere* p, xIsect* isx);
void iSphereInitBoundOBB(xSphere* s, const xBox* b, const xMat4x3* m);
void iCylinderIsectRay(const xCylinder* c, const xRay3* r, xIsect* isx);
void iBoxIsectVec(const xBox* b, const xVec3* v, xIsect* isx);
void iBoxIsectRay(const xBox* b, const xRay3* r, xIsect* isx);
void iBoxIsectSphere(const xBox* box, const xSphere* p, xIsect* isx);

#endif
