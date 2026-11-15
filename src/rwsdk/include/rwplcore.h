#ifndef RWPLCORE_H
#define RWPLCORE_H

#include <stdarg.h>
#include <stddef.h>

typedef int RwInt32;
typedef unsigned int RwUInt32;
typedef unsigned short RwUInt16;
typedef unsigned char RwUInt8;
typedef char RwChar;
typedef float RwReal;
typedef RwInt32 RwBool;

#define RWFORCEENUMSIZEINT ((RwInt32)((~((RwUInt32)0)) >> 1))

#define MACRO_START do
#define MACRO_STOP while (0)

typedef struct RwDevice RwDevice;
typedef struct RwFileFunctions RwFileFunctions;
typedef struct RwFreeList RwFreeList;
typedef struct RwGlobals RwGlobals;
typedef struct RwLLLink RwLLLink;
typedef struct RwLine RwLine;
typedef struct RwLinkList RwLinkList;
typedef struct RwMemoryFunctions RwMemoryFunctions;
typedef struct RwMetrics RwMetrics;
typedef struct RwObject RwObject;
typedef struct RwRGBA RwRGBA;
typedef struct RwSphere RwSphere;
typedef struct RwStringFunctions RwStringFunctions;
typedef struct RwV3d RwV3d;
typedef struct rwGameCube2DVertex rwGameCube2DVertex;

typedef struct RwMatrixTag RwMatrix;
typedef rwGameCube2DVertex RwIm2DVertex;
typedef RwUInt16 RxVertexIndex;
typedef RxVertexIndex RwImVertexIndex;

struct RwV3d
{
    RwReal x;
    RwReal y;
    RwReal z;
};

struct RwLLLink
{
    RwLLLink* next;
    RwLLLink* prev;
};

struct RwLinkList
{
    RwLLLink link;
};

struct RwMemoryFunctions
{
    void* (*rwmalloc)(size_t size);
    void (*rwfree)(void* mem);
    void* (*rwrealloc)(void* mem, size_t newSize);
    void* (*rwcalloc)(size_t numObj, size_t sizeObj);
};

struct RwMatrixTag
{
    RwV3d right;
    RwUInt32 flags;
    RwV3d up;
    RwUInt32 pad1;
    RwV3d at;
    RwUInt32 pad2;
    RwV3d pos;
    RwUInt32 pad3;
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

struct RwRGBA
{
    RwUInt8 red;
    RwUInt8 green;
    RwUInt8 blue;
    RwUInt8 alpha;
};

struct rwGameCube2DVertex
{
    RwReal x;
    RwReal y;
    RwReal z;
    RwUInt8 r;
    RwUInt8 g;
    RwUInt8 b;
    RwUInt8 a;
    RwReal u;
    RwReal v;
    RwReal recipz;
};

enum RwRenderState
{
    rwRENDERSTATENARENDERSTATE,
    rwRENDERSTATETEXTURERASTER,
    rwRENDERSTATETEXTUREADDRESS,
    rwRENDERSTATETEXTUREADDRESSU,
    rwRENDERSTATETEXTUREADDRESSV,
    rwRENDERSTATETEXTUREPERSPECTIVE,
    rwRENDERSTATEZTESTENABLE,
    rwRENDERSTATESHADEMODE,
    rwRENDERSTATEZWRITEENABLE,
    rwRENDERSTATETEXTUREFILTER,
    rwRENDERSTATESRCBLEND,
    rwRENDERSTATEDESTBLEND,
    rwRENDERSTATEVERTEXALPHAENABLE,
    rwRENDERSTATEBORDERCOLOR,
    rwRENDERSTATEFOGENABLE,
    rwRENDERSTATEFOGCOLOR,
    rwRENDERSTATEFOGTYPE,
    rwRENDERSTATEFOGDENSITY,
    rwRENDERSTATEFOGTABLE,
    rwRENDERSTATEALPHAPRIMITIVEBUFFER,
    rwRENDERSTATECULLMODE,
    rwRENDERSTATESTENCILENABLE,
    rwRENDERSTATESTENCILFAIL,
    rwRENDERSTATESTENCILZFAIL,
    rwRENDERSTATESTENCILPASS,
    rwRENDERSTATESTENCILFUNCTION,
    rwRENDERSTATESTENCILFUNCTIONREF,
    rwRENDERSTATESTENCILFUNCTIONMASK,
    rwRENDERSTATESTENCILFUNCTIONWRITEMASK,
    rwRENDERSTATEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT,
};
typedef enum RwRenderState RwRenderState;

enum RwFogType
{
    rwFOGTYPENAFOGTYPE = 0,
    rwFOGTYPELINEAR,
    rwFOGTYPEEXPONENTIAL,
    rwFOGTYPEEXPONENTIAL2,
    rwFOGTYPEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT,
};
typedef enum RwFogType RwFogType;

enum RwBlendFunction
{
    rwBLENDNABLEND,
    rwBLENDZERO,
    rwBLENDONE,
    rwBLENDSRCCOLOR,
    rwBLENDINVSRCCOLOR,
    rwBLENDSRCALPHA,
    rwBLENDINVSRCALPHA,
    rwBLENDDESTALPHA,
    rwBLENDINVDESTALPHA,
    rwBLENDDESTCOLOR,
    rwBLENDINVDESTCOLOR,
    rwBLENDSRCALPHASAT,
    rwBLENDFUNCTIONFORCEENUMSIZEINT = RWFORCEENUMSIZEINT,
};
typedef enum RwBlendFunction RwBlendFunction;

enum RwPrimitiveType
{
    rwPRIMTYPENAPRIMTYPE,
    rwPRIMTYPELINELIST,
    rwPRIMTYPEPOLYLINE,
    rwPRIMTYPETRILIST,
    rwPRIMTYPETRISTRIP,
    rwPRIMTYPETRIFAN,
    rwPRIMTYPEPOINTLIST,
    rwPRIMITIVETYPEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT,
};
typedef enum RwPrimitiveType RwPrimitiveType;

typedef RwBool (*RwSystemFunc)(RwInt32 nOption, void* pOut, void* pInOut, RwInt32 nIn);
typedef RwBool (*RwStandardFunc)(void* pOut, void* pInOut, RwInt32 nI);
typedef void* (*RwMemoryAllocFn)(RwFreeList* fl);
typedef RwFreeList* (*RwMemoryFreeFn)(RwFreeList* fl, void* pData);

typedef RwBool (*RwRenderStateSetFunction)(RwRenderState nState, void* pParam);
typedef RwBool (*RwRenderStateGetFunction)(RwRenderState nState, void* pParam);
typedef RwBool (*RwIm2DRenderLineFunction)(RwIm2DVertex* vertices, RwInt32 numVertices, RwInt32 vert1, RwInt32 vert2);
typedef RwBool (*RwIm2DRenderTriangleFunction)(RwIm2DVertex* vertices, RwInt32 numVertices, RwInt32 vert1, RwInt32 vert2, RwInt32 vert3);
typedef RwBool (*RwIm2DRenderPrimitiveFunction)(RwPrimitiveType primType, RwIm2DVertex* vertices, RwInt32 numVertices);
typedef RwBool (*RwIm2DRenderIndexedPrimitiveFunction)(RwPrimitiveType primType, RwIm2DVertex* vertices, RwInt32 numVertices, RwImVertexIndex* indices, RwInt32 numIndices);
typedef RwBool (*RwIm3DRenderLineFunction)(RwInt32 vert1, RwInt32 vert2);
typedef RwBool (*RwIm3DRenderTriangleFunction)(RwInt32 vert1, RwInt32 vert2, RwInt32 vert3);
typedef RwBool (*RwIm3DRenderPrimitiveFunction)(RwPrimitiveType primType);
typedef RwBool (*RwIm3DRenderIndexedPrimitiveFunction)(RwPrimitiveType primtype, RwImVertexIndex* indices, RwInt32 numIndices);

struct RwDevice
{
    RwReal gammaCorrection;
    RwSystemFunc fpSystem;
    RwReal zBufferNear;
    RwReal zBufferFar;
    RwRenderStateSetFunction fpRenderStateSet;
    RwRenderStateGetFunction fpRenderStateGet;
    RwIm2DRenderLineFunction fpIm2DRenderLine;
    RwIm2DRenderTriangleFunction fpIm2DRenderTriangle;
    RwIm2DRenderPrimitiveFunction fpIm2DRenderPrimitive;
    RwIm2DRenderIndexedPrimitiveFunction fpIm2DRenderIndexedPrimitive;
    RwIm3DRenderLineFunction fpIm3DRenderLine;
    RwIm3DRenderTriangleFunction fpIm3DRenderTriangle;
    RwIm3DRenderPrimitiveFunction fpIm3DRenderPrimitive;
    RwIm3DRenderIndexedPrimitiveFunction fpIm3DRenderIndexedPrimitive;
};

typedef RwBool (*rwFnFexist)(const RwChar* name);
typedef void* (*rwFnFopen)(const RwChar* name, const RwChar* mode);
typedef int (*rwFnFclose)(void* fptr);
typedef size_t (*rwFnFread)(void* addr, size_t size, size_t count, void* fptr);
typedef size_t (*rwFnFwrite)(const void* addr, size_t size, size_t count, void* fptr);
typedef RwChar* (*rwFnFgets)(RwChar* buffer, int maxLen, void* fptr);
typedef int (*rwFnFputs)(const RwChar* buffer, void* fptr);
typedef int (*rwFnFeof)(void* fptr);
typedef int (*rwFnFseek)(void* fptr, long offset, int origin);
typedef int (*rwFnFflush)(void* fptr);
typedef int (*rwFnFtell)(void* fptr);

struct RwFileFunctions
{
    rwFnFexist rwfexist;
    rwFnFopen rwfopen;
    rwFnFclose rwfclose;
    rwFnFread rwfread;
    rwFnFwrite rwfwrite;
    rwFnFgets rwfgets;
    rwFnFputs rwfputs;
    rwFnFeof rwfeof;
    rwFnFseek rwfseek;
    rwFnFflush rwfflush;
    rwFnFtell rwftell;
};

typedef int (*vecSprintfFunc)(RwChar* buffer, const RwChar* format, ...);
typedef int (*vecVsprintfFunc)(RwChar* buffer, const RwChar* format, va_list argptr);
typedef RwChar* (*vecStrcpyFunc)(RwChar* dest, const RwChar* srce);
typedef RwChar* (*vecStrncpyFunc)(RwChar* dest, const RwChar* srce, size_t size);
typedef RwChar* (*vecStrcatFunc)(RwChar* dest, const RwChar* srce);
typedef RwChar* (*vecStrncatFunc)(RwChar* dest, const RwChar* srce, size_t size);
typedef RwChar* (*vecStrrchrFunc)(const RwChar* string, int findThis);
typedef RwChar* (*vecStrchrFunc)(const RwChar* string, int findThis);
typedef RwChar* (*vecStrstrFunc)(const RwChar* string, const RwChar* findThis);
typedef int (*vecStrcmpFunc)(const RwChar* string1, const RwChar* string2);
typedef int (*vecStricmpFunc)(const RwChar* string1, const RwChar* string2);
typedef size_t (*vecStrlenFunc)(const RwChar* string);
typedef RwChar* (*vecStruprFunc)(RwChar* string);
typedef RwChar* (*vecStrlwrFunc)(RwChar* string);
typedef RwChar* (*vecStrtokFunc)(RwChar* string, const RwChar* delimit);
typedef int (*vecSscanfFunc)(const RwChar* buffer, const RwChar* format, ...);

struct RwStringFunctions
{
    vecSprintfFunc vecSprintf;
    vecVsprintfFunc vecVsprintf;
    vecStrcpyFunc vecStrcpy;
    vecStrncpyFunc vecStrncpy;
    vecStrcatFunc vecStrcat;
    vecStrncatFunc vecStrncat;
    vecStrrchrFunc vecStrrchr;
    vecStrchrFunc vecStrchr;
    vecStrstrFunc vecStrstr;
    vecStrcmpFunc vecStrcmp;
    vecStricmpFunc vecStricmp;
    vecStrlenFunc vecStrlen;
    vecStruprFunc vecStrupr;
    vecStrlwrFunc vecStrlwr;
    vecStrtokFunc vecStrtok;
    vecSscanfFunc vecSscanf;
};

enum RwEngineStatus
{
    rwENGINESTATUSIDLE,
    rwENGINESTATUSINITED,
    rwENGINESTATUSOPENED,
    rwENGINESTATUSSTARTED,
    rwENGINESTATUSFORCEENUMSIZEINT = RWFORCEENUMSIZEINT,
};
typedef enum RwEngineStatus RwEngineStatus;

enum RwDebugType
{
    rwNADEBUGTYPE,
    rwDEBUGASSERT,
    rwDEBUGERROR,
    rwDEBUGMESSAGE,
    rwDEBUGTRACE,
    rwDEBUGTYPEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT,
};
typedef enum RwDebugType RwDebugType;

typedef void (*RwDebugHandler)(RwDebugType type, const RwChar* string);

#define rwSTANDARDNUMOFSTANDARD 29

struct RwGlobals
{
#ifdef RWDEBUG
    RwDebugHandler debugFunction;
    void* debugFile;
    RwInt32 debugStackDepth;
    RwBool debugTrace;
#endif
    void* curCamera;
    void* curWorld;
    RwUInt16 renderFrame;
    RwUInt16 lightFrame;
    RwUInt16 pad[2];
    RwDevice dOpenDevice;
    RwStandardFunc stdFunc[rwSTANDARDNUMOFSTANDARD];
    RwLinkList dirtyFrameList;
    RwFileFunctions fileFuncs;
    RwStringFunctions stringFuncs;
    RwMemoryFunctions memoryFuncs;
#ifdef RWDEBUG
    RwBool freeListExtraDebug;
#endif
    RwMemoryAllocFn memoryAlloc;
    RwMemoryFreeFn memoryFree;
    RwMetrics* metrics;
    RwEngineStatus engineStatus;
    RwUInt32 resArenaInitSize;
};

struct RwObject
{
    RwUInt8 type;
    RwUInt8 subType;
    RwUInt8 flags;
    RwUInt8 privateFlags;
    void* parent;
};

#define RWSRCGLOBAL(variable) (((RwGlobals *)RwEngineInstance)->variable)
#define rwObjectGetParent(object) (((const RwObject *)(object))->parent)

#define RwIm2DVertexSetIntRGBA(vert, red, green, blue, alpha)  \
    MACRO_START                                                \
    {                                                          \
        ((vert)->r = (red));                                   \
        ((vert)->g = (green));                                 \
        ((vert)->b = (blue));                                  \
        ((vert)->a = (alpha));                                 \
    }                                                          \
    MACRO_STOP

#define RwIm2DVertexSetScreenX(vert, scrnx) ((vert)->x = (scrnx))
#define RwIm2DVertexSetScreenY(vert, scrny) ((vert)->y = (scrny))
#define RwIm2DVertexSetScreenZ(vert, scrnz) ((vert)->z = (scrnz))

#define RwV3dAddMacro(o, a, b)            \
    MACRO_START                           \
    {                                     \
        (o)->x = (((a)->x) + ( (b)->x));  \
        (o)->y = (((a)->y) + ( (b)->y));  \
        (o)->z = (((a)->z) + ( (b)->z));  \
    }                                     \
    MACRO_STOP

#ifdef __cplusplus
extern "C" {
#endif

extern void* RwEngineInstance;

RwBool RwRenderStateSet(RwRenderState state, void* value);

RwReal RwIm2DGetNearScreenZ(void);
RwBool RwIm2DRenderLine(RwIm2DVertex* vertices, RwInt32 numVertices, RwInt32 vert1, RwInt32 vert2);

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
void RwV3dAdd(RwV3d* out, const RwV3d* ina, const RwV3d* inb);
#endif

#ifdef __cplusplus
}
#endif

#if !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE))
#define RwV3dAdd(o, a, b) RwV3dAddMacro(o, a, b)
#endif

#endif
