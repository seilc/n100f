#ifndef XUTIL_H
#define XUTIL_H

#include "types.h"

#define IDTAG(a,b,c,d) (((U32)(a)<<24)|((U32)(b)<<16)|((U32)(c)<<8)|((U32)(d)))
#define ARRAY_LENGTH(a) (sizeof(a) / sizeof(a[0]))
#define SCENE_ID(s) IDTAG((s)[0] & ~0x20, (s)[1], (s)[2], (s)[3])

S32 xUtilStartup();
S32 xUtilShutdown();
char* xUtil_idtag2string(U32 srctag, S32 bufidx);
U32 xUtil_crc_init();
U32 xUtil_crc_update(U32 crc_accum, char* data, S32 datasize);
void strtosjis(U8* string, U8* dest); // UNUSED
U8 BCDtoi(U8 hex); // UNUSED
U8 itoBCD(U8 dec); // UNUSED
U8 itoBCD(U16 dec); // UNUSED
S32 xUtil_yesno(F32 wt_yes);
S32 xUtil_yesno();

#endif
