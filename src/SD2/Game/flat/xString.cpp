#include "xCommon.h"
#include "xString.h"

#include <stdlib.h>

#define CHAR_MAP_SIZE 32

U32 xStrHash(char* str)
{
    U32 i;

    for (i = 0; *str != '\0'; ++str) {
        char c = *str;
        c = c - (c & (c >> 1) & 0x20);
        i = i * 131 + c;
    }
    return i;
}

char* xStrTok(char* string, const char* control, char** nextoken)
{
    U8* str;
    const U8* ctrl = (const U8*)control;
    U8 map[CHAR_MAP_SIZE];

    for (S32 count = 0; count < CHAR_MAP_SIZE; ++count)
        map[count] = 0;
    do {
        map[*ctrl >> 3] |= (U8)(1 << (*ctrl & 0x7));
    } while (*ctrl++);

    if (string)
        str = (U8*)string;
    else
        str = (U8*)*nextoken;
    while ((map[*str >> 3] & (1 << (*str & 0x7))) && *str)
        ++str;

    string = (char*)str;

    while (*str) {
        if (map[*str >> 3] & (1 << (*str & 0x7))) {
            *str = '\0'; ++str;
            break;
        }
        ++str;
    }

    *nextoken = (char*)str;

    if (string == (char*)str) {
        return NULL;
    }

    return string;
}

char* xStrTokBuffer(const char* string, const char* control, void* buffer)
{
    char** nextoken = (char**)buffer;
    char* dest = (char*)(nextoken + 1);
    const U8* str;
    const U8* ctrl = (const U8*)control;
    U8 map[CHAR_MAP_SIZE];

    for (S32 count = 0; count < CHAR_MAP_SIZE; ++count)
        map[count] = 0;
    do {
        map[*ctrl >> 3] |= (U8)(1 << (*ctrl & 0x7));
    } while (*ctrl++);

    if (string)
        str = (const U8*)string;
    else
        str = (const U8*)*nextoken;
    while ((map[*str >> 3] & (1 << (*str & 0x7))) && *str)
        ++str;

    string = (const char*)str;

    for (; *str; ++str) {
        if (map[*str >> 3] & (1 << (*str & 0x7))) {
            ++str;
            break;
        }

        *dest = *str;
        ++dest;
    }

    *dest = '\0';
    *nextoken = (char*)str;

    if (string == (const char*)str) {
        return NULL;
    }

    return (char*)(nextoken + 1);
}

#define xToUpper(c) (((c) >= 'a' && (c) <= 'z') ? (c) - 32 : (c))

S32 xStricmp(const char* string1, const char* string2)
{
    S32 result = 0;

    while (xToUpper(*string1) == xToUpper(*string2) && result == 0) {
        if (*string1 == '\0' || *string2 == '\0') {
            result = 1;
        } else {
            ++string1;
            ++string2;
        }
    }

    result = 0;
    if (*string1 != *string2) {
        result = 1;
        if (xToUpper(*string1) < xToUpper(*string2)) {
            result = -1;
        }
    }

    return result;
}

char* xStrupr(char* string)
{
    char* p = string;
    while (*p) {
        *p = xToUpper(*p);
        ++p;
    }
    return string;
}

S32 xStrParseFloatList(F32* dest, const char* strbuf, S32 max)
{
    char* str = (char*)strbuf;
    if (str == NULL) return 0;

    S32 index = 0;

    while (*str && index < max) {
        while (*str == '\t' ||
               *str == ' ' ||
               *str == '+' ||
               *str == '[' ||
               *str == ']' ||
               *str == '{' ||
               *str == '}' ||
               *str == '(' ||
               *str == ')' ||
               *str == ',' ||
               *str == ':' ||
               *str == ';') {
            ++str;
        }

        if (*str == '\0') {
            return index;
        }

        S32 digits;
        S32 negate;
        char tmpc;
        char* numstart;

        if (*str == '-') {
            negate = TRUE;
            ++str;
            while (*str == '\t' || *str == ' ')
                ++str;
        } else {
            negate = FALSE;
        }

        numstart = str;
        digits = 0;

        while ((*str >= '0' && *str <= '9') ||
               *str == '.' ||
               *str == 'e' ||
               *str == 'E' ||
               *str == 'f') {
            if (*str >= '0' && *str <= '9')
                ++digits;
            ++str;
        }

        if (digits == 0) {
            return index;
        }

        tmpc = *str;
        *str = '\0';

        dest[index] = atof(numstart);

        if (negate) dest[index] = -dest[index];

        *str = tmpc; ++index;
    }

    return index;
}
