#ifndef ZFMV_H
#define ZFMV_H

#include "types.h"

enum eFMVFile
{
    eFMVFile_HeavyIronLOGO,
    eFMVFile_SDTitleSequence,
    eFMVFile_Demo1,
    eFMVFile_Demo2,
    eFMVFile_Demo3,
    eFMVFile_Demo4,
    eFMVFile_Demo5,
    eFMVFile_Demo6,
    eFMVFile_Demo7,
    eFMVFile_Demo8,
    eFMVFile_Demo9,
    eFMVFile_Demo10,
    eFMVFile_Demo11,
    eFMVFile_Demo12,
    eFMVFile_Demo13,
    eFMVFile_Demo14,
    eFMVFile_Demo15,
    eFMVFile_THQLOGO,
    eFMVFile_WarnerBrosLOGO,
    eFMVFile_WBExtra0,
    eFMVFile_WBExtra1,
    eFMVFile_WBExtra2,
    eFMVFile_WBExtra3,
    eFMVFile_WBExtra4,
    eFMVFile_Credits,
    eFMVFile_EndCredits,
    eFMVFile_RenderWareLOGO,
    eFMVFile_Cin1,
    eFMVFile_Cin2,
    eFMVFile_Cin3,
    eFMVFile_Cin4,
    eFMVFile_Cin5,
    eFMVFile_Cin6,
    eFMVFile_Cin7,
    eFMVFile_Cin8,
    eFMVFile_Cin9,
    eFMVFile_Cin10,
    eFMVFile_Cin11,
    eFMVFile_Cin12,
    eFMVFile_Cin13,
    eFMVFile_Cin14,
    eFMVFileCount
};

struct zFMVFile
{
    eFMVFile type;
    char fileName[32];
    U32 fileNameID;
};

extern U32 gCreditsBlackout;

char* zFMVFileGetName(eFMVFile fileEnum);
U32 zFMVPlay(char* filename, U32 buttons, F32 time);

#endif
