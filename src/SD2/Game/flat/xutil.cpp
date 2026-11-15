#include "xCommon.h"
#include "xutil.h"

#include "iMath.h"

#include <ctype.h>

static S32 g_xutilinit;
static S32 g_crc_needinit = TRUE;
#define CRC32_TABLE_SIZE 256

static U32 g_crc32_table[CRC32_TABLE_SIZE] = { 0 };

S32 xUtilStartup()
{
    if (g_xutilinit++ == 0) {
        xUtil_crc_init();
    }

    return g_xutilinit;
}

S32 xUtilShutdown()
{
    --g_xutilinit;
    return g_xutilinit;
}

char* xUtil_idtag2string(U32 srctag, S32 bufidx)
{
    U32 tag = srctag;
    char* strptr = NULL;
    char* uc = (char*)&tag;
    static char buf[6][10] = { 0 };

    if (bufidx < 0 || bufidx >= 7) strptr = buf[0];
    else strptr = buf[bufidx];

    S32 l = 1;
    if (((char*)&l)[3]) {
        char t = 0;
        t = uc[0]; uc[0] = uc[3]; uc[3] = t;
        t = uc[1]; uc[1] = uc[2]; uc[2] = t;
    }

    switch (bufidx) {
    case 4:
    case 5:
        strptr[0] = isprint(uc[0]) ? uc[0] : '?';
        strptr[1] = isprint(uc[1]) ? uc[1] : '?';
        strptr[2] = isprint(uc[2]) ? uc[2] : '?';
        strptr[3] = isprint(uc[3]) ? uc[3] : '?';
        break;
    case 6:
    default:
        strptr[0] = isprint(uc[3]) ? uc[3] : '?';
        strptr[1] = isprint(uc[2]) ? uc[2] : '?';
        strptr[2] = isprint(uc[1]) ? uc[1] : '?';
        strptr[3] = isprint(uc[0]) ? uc[0] : '?';
        break;
    }

    strptr[4] = '\0';

    if (bufidx == 6) {
        strptr[4] = '/';
        strptr[5] = isprint(uc[0]) ? uc[0] : '?';
        strptr[6] = isprint(uc[1]) ? uc[1] : '?';
        strptr[7] = isprint(uc[2]) ? uc[2] : '?';
        strptr[8] = isprint(uc[3]) ? uc[3] : '?';
        strptr[9] = '\0';
    }

    return strptr;
}

U32 xUtil_crc_init()
{
    S32 i = 0, j = 0;
    U32 crc_accum = 0;

    if (g_crc_needinit) {
        for (i = 0; i < CRC32_TABLE_SIZE; i++) {
            crc_accum = (U32)i << 24;
            for (j = 0; j < 8; j++) {
                if (crc_accum & 0x80000000) {
                    crc_accum = (crc_accum << 1) ^ 0x04C11DB7;
                } else {
                    crc_accum = crc_accum << 1;
                }
            }
            g_crc32_table[i] = crc_accum;
        }

        g_crc_needinit = FALSE;
    }

    return 0xFFFFFFFF;
}

U32 xUtil_crc_update(U32 crc_accum, char* data, S32 datasize)
{
    S32 i = 0, j = 0;

    if (g_crc_needinit) {
        xUtil_crc_init();
    }

    for (j = 0; j < datasize; j++) {
        i = ((S32)(crc_accum >> 24) ^ *data++) & 0xFF;
        crc_accum = (crc_accum << 8) ^ g_crc32_table[i];
    }

    return crc_accum;
}

S32 xUtil_yesno()
{
    return xUtil_yesno(0.5f);
}

S32 xUtil_yesno(F32 wt_yes)
{
    if (wt_yes == 0.0f) return FALSE;
    if (wt_yes == 1.0f) return TRUE;
    if (iurand() <= wt_yes) return TRUE;
    return FALSE;
}
