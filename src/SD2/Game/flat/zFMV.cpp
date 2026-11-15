#include "xCommon.h"
#include "zFMV.h"

#include "iFMV.h"
#include "xSnd.h"
#include "xString.h"
#include "xutil.h"
#include "zGlobals.h"

#include <stdio.h>

static zFMVFile zFMVFileTable[eFMVFileCount] =
{
    { eFMVFile_HeavyIronLOGO, "HILogo", 0 },
    { eFMVFile_SDTitleSequence, "Intro", 0 },
    { eFMVFile_Demo1, "demo/demo1", 0 },
    { eFMVFile_Demo2, "demo/demo2", 0 },
    { eFMVFile_Demo3, "Intro", 0 },
    { eFMVFile_Demo4, "demo/demo1", 0 },
    { eFMVFile_Demo5, "demo/demo2", 0 },
    { eFMVFile_Demo6, "Intro", 0 },
    { eFMVFile_Demo7, "demo/demo1", 0 },
    { eFMVFile_Demo8, "demo/demo2", 0 },
    { eFMVFile_Demo9, "Intro", 0 },
    { eFMVFile_Demo10, "demo/demo1", 0 },
    { eFMVFile_Demo11, "demo/demo2", 0 },
    { eFMVFile_Demo12, "Intro", 0 },
    { eFMVFile_Demo13, "demo/demo1", 0 },
    { eFMVFile_Demo14, "demo/demo2", 0 },
    { eFMVFile_Demo15, "Intro", 0 },
    { eFMVFile_THQLOGO, "THQLogo", 0 },
    { eFMVFile_WarnerBrosLOGO, "WBLogo", 0 },
    { eFMVFile_WBExtra0, "SDTrailr", 0 },
    { eFMVFile_WBExtra1, "HWTrailr", 0 },
    { eFMVFile_WBExtra2, "HILogo", 0 },
    { eFMVFile_WBExtra3, "HILogo", 0 },
    { eFMVFile_WBExtra4, "HILogo", 0 },
    { eFMVFile_Credits, "Credits1", 0 },
    { eFMVFile_EndCredits, "Credits2", 0 },
    { eFMVFile_RenderWareLOGO, "RW4", 0 },
    { eFMVFile_Cin1, "cin1", 0 },
    { eFMVFile_Cin2, "cin2", 0 },
    { eFMVFile_Cin3, "cin3", 0 },
    { eFMVFile_Cin4, "cin4", 0 },
    { eFMVFile_Cin5, "cin5", 0 },
    { eFMVFile_Cin6, "cin6", 0 },
    { eFMVFile_Cin7, "cin7", 0 },
    { eFMVFile_Cin8, "cin8", 0 },
    { eFMVFile_Cin9, "cin9", 0 },
    { eFMVFile_Cin10, "cin10", 0 },
    { eFMVFile_Cin11, "cin11", 0 },
    { eFMVFile_Cin12, "cin_mmpd", 0 },
    { eFMVFile_Cin13, "cin_mmsc", 0 },
    { eFMVFile_Cin14, "cin_shbr", 0 }
};

U32 gCreditsBlackout;

U32 zFMVPlay(char* filename, U32 buttons, F32 time)
{
    U32 ret;
    char c[64];
    sprintf(c, "%s/%s%s", "FMV", filename, ".bik");
    xSndSuspend();
    ret = iFMVPlay(c, buttons, time);
    xSndResume();
    if (xStricmp(filename, "Credits2") == 0 && globals.sceneCur->sceneID == SCENE_ID("s005")) {
        gCreditsBlackout = TRUE;
    }
    return ret;
}

char* zFMVFileGetName(eFMVFile fileEnum)
{
    return zFMVFileTable[fileEnum].fileName;
}
