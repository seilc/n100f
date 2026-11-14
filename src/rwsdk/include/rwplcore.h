#ifndef RWPLCORE_H
#define RWPLCORE_H

typedef int RwInt32;
typedef unsigned int RwUInt32;
typedef float RwReal;

#define RWFORCEENUMSIZEINT ((RwInt32)((~((RwUInt32)0)) >> 1))

typedef struct RwLine RwLine;
typedef struct RwSphere RwSphere;
typedef struct RwV3d RwV3d;
typedef struct RwMatrixTag RwMatrix;

struct RwV3d
{
    RwReal x;
    RwReal y;
    RwReal z;
};

struct RwSphere
{
    RwV3d center;
    RwReal radius;
};

struct RwLine
{
    RwV3d start;
    RwV3d end;
};

#endif
