#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <omega/platform.h>
#if OMG_HAS_STD
#include <stdio.h>
#endif

#define OMG_ISNULL(ptr) (ptr == NULL)
#define OMG_ISNOTNULL(ptr) (ptr != NULL)

#define _OMG_UNUSED1(p1) ((void)p1)
#define _OMG_UNUSED2(p1, p2) ((void)p1, (void)p2)
#define _OMG_UNUSED3(p1, p2, p3) ((void)p1, (void)p2, (void)p3)
#define _OMG_UNUSED4(p1, p2, p3, p4) ((void)p1, (void)p2, (void)p3, (void)p4)
#define _OMG_UNUSED5(p1, p2, p3, p4, p5) ((void)p1, (void)p2, (void)p3, (void)p4, (void)p5)
#define _OMG_UNUSED6(p1, p2, p3, p4, p5, p6) ((void)p1, (void)p2, (void)p3, (void)p4, (void)p5, (void)p6)
#define _OMG_UNUSED7(p1, p2, p3, p4, p5, p6, p7) ((void)p1, (void)p2, (void)p3, (void)p4, (void)p5, (void)p6, (void)p7)
#define _OMG_UNUSED8(p1, p2, p3, p4, p5, p6, p7, p8) ((void)p1, (void)p2, (void)p3, (void)p4, (void)p5, (void)p6, (void)p7, (void)p8)
#define _OMG_UNUSED_INTERNAL(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME
#define OMG_UNUSED(...) _OMG_UNUSED_INTERNAL(__VA_ARGS__, _OMG_UNUSED8, _OMG_UNUSED7, _OMG_UNUSED6, _OMG_UNUSED5, _OMG_UNUSED4, _OMG_UNUSED3, _OMG_UNUSED2, _OMG_UNUSED1)(__VA_ARGS__)

#ifdef _MSC_VER

#define OMG_BEGIN_POINTER_CAST() do { \
    _Pragma("warning( disable : 4113 )") \
    _Pragma("warning( disable : 4133 )") \
} while (0)

#define OMG_END_POINTER_CAST() do { \
    _Pragma("warning( default : 4113 )") \
    _Pragma("warning( default : 4133 )")
} while (0)

#else

#define OMG_BEGIN_POINTER_CAST() do { \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wincompatible-pointer-types\"") \
} while (0)

#define OMG_END_POINTER_CAST() do { \
    _Pragma("GCC diagnostic pop") \
} while (0)

OMG_API void* omg_memset(void *dest, register int val, register size_t len);
OMG_API void* omg_memcpy(void *dest, const void* src, size_t len);
OMG_API size_t omg_strlen(const char* src);

#endif
