#ifndef _DOLPHIN_LIBC_CTYPE_H_
#define _DOLPHIN_LIBC_CTYPE_H_

#define __control_char 0x01
#define __motion_char  0x02
#define __space_char   0x04
#define __punctuation  0x08
#define __digit        0x10
#define __hex_digit    0x20
#define __lower_case   0x40
#define __upper_case   0x80

#define __letter       (__lower_case | __upper_case)
#define __alphanumeric (__letter | __digit)
#define __graphic      (__alphanumeric | __punctuation)
#define __printable    (__graphic | __space_char)

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char __ctype_map[];

inline int isprint(int c) { return (int)(__ctype_map[(unsigned char)c] & __printable); }

#ifdef __cplusplus
}
#endif

int tolower(int c);

#endif // _DOLPHIN_LIBC_CTYPE_H_
