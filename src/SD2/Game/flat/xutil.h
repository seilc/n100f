#ifndef XUTIL_H
#define XUTIL_H

#define IDTAG(a,b,c,d) (((U32)(a)<<24)|((U32)(b)<<16)|((U32)(c)<<8)|((U32)(d)))
#define ARRAY_LENGTH(a) (sizeof(a) / sizeof(a[0]))
#define SCENE_ID(s) IDTAG((s)[0] & ~0x20, (s)[1], (s)[2], (s)[3])

#endif
