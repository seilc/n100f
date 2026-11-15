#ifndef XSTRING_H
#define XSTRING_H

#include "types.h"

U32 xStrHash(char* str);
char* xStrTok(char* string, const char* control, char** nextoken);
char* xStrTokBuffer(const char* string, const char* control, void* buffer);
S32 xStricmp(const char* string1, const char* string2);
char* xStrupr(char* string);
S32 xStrParseFloatList(F32* dest, const char* strbuf, S32 max);

#endif
