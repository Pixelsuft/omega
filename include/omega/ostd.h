#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <omega/platform.h>
#if OMG_HAS_STD
#include <stdio.h>
#endif

#define OMG_ISNULL(ptr) (ptr == NULL)
#define OMG_ISNOTNULL(ptr) (ptr != NULL)

#define OMG_STRING_NONE 0
#define OMG_STRING_STATIC 1
#define OMG_STRING_BUFFER 2
#define OMG_STRING_DYNAMIC 3
// TODO: use << and >> to speedup
#define OMG_STRING_CHUNK_SIZE 16
#define OMG_STRING_MAKE_STATIC(char_ptr) ((OMG_String){ .type = OMG_STRING_STATIC, .len = omg_strlen(char_ptr), .size = omg_strlen(char_ptr) + 1, .ptr = char_ptr })
#define OMG_STRING_MAKE_BUFFER(char_ptr) ((OMG_String){ .type = OMG_STRING_BUFFER, .len = omg_strlen(char_ptr), .size = omg_strlen(char_ptr) + 1, .ptr = char_ptr })
#define OMG_STRING_MAKE_BUFFER_A(char_ptr) ((OMG_String){ .type = OMG_STRING_BUFFER, .len = sizeof(char_ptr) - 1, .size = sizeof(char_ptr), .ptr = char_ptr })
#define OMG_STRING_CALC_SIZE_BY_LENGTH(str_len) ((str_len % OMG_STRING_CHUNK_SIZE) ? ((str_len / OMG_STRING_CHUNK_SIZE + 1) * OMG_STRING_CHUNK_SIZE) : (str_len ? str_len : OMG_STRING_CHUNK_SIZE))

#define _OMG_STRING_GET_ADD_FUNC(X) _Generic((X), \
    char*: omg_string_add_char_p, \
    OMG_String*: omg_string_add, \
    OMG_String: omg_string_add \
)
#define _OMG_STRING_GET_ADD_VALUE(X) _Generic((X), \
    OMG_String: &X, \
    default: X \
)

#define OMG_STRING_ADD(string, to_add) (_OMG_STRING_GET_ADD_FUNC(to_add)(string, _OMG_STRING_GET_ADD_VALUE(to_add)))
#define _OMG_STRING_ADD2(s, p1, p2) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); } while(0)
#define _OMG_STRING_ADD3(s, p1, p2, p3) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); } while(0)
#define _OMG_STRING_ADD4(s, p1, p2, p3, p4) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); } while(0)
#define _OMG_STRING_ADD5(s, p1, p2, p3, p4, p5) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); } while(0)
#define _OMG_STRING_ADD6(s, p1, p2, p3, p4, p5, p6) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); } while(0)
#define _OMG_STRING_ADD7(s, p1, p2, p3, p4, p5, p6, p7) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); } while(0)
#define _OMG_STRING_ADD8(s, p1, p2, p3, p4, p5, p6, p7, p8) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); } while(0)
#define _OMG_STRING_ADD9(s, p1, p2, p3, p4, p5, p6, p7, p8, p9) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); } while(0)
#define _OMG_STRING_ADD10(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); } while(0)
#define _OMG_STRING_ADD11(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); } while(0)
#define _OMG_STRING_ADD12(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); OMG_STRING_ADD(s, p12); } while(0)
#define _OMG_STRING_ADD13(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); OMG_STRING_ADD(s, p12); OMG_STRING_ADD(s, p13); } while(0)
#define _OMG_STRING_ADD14(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); OMG_STRING_ADD(s, p12); OMG_STRING_ADD(s, p13); OMG_STRING_ADD(s, p14); } while(0)
#define _OMG_STRING_ADD15(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); OMG_STRING_ADD(s, p12); OMG_STRING_ADD(s, p13); OMG_STRING_ADD(s, p14); OMG_STRING_ADD(s, p15); } while(0)
#define _OMG_STRING_ADD16(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); OMG_STRING_ADD(s, p12); OMG_STRING_ADD(s, p13); OMG_STRING_ADD(s, p14); OMG_STRING_ADD(s, p15); OMG_STRING_ADD(s, p16); } while(0)
#define _OMG_STRING_ADD17(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); OMG_STRING_ADD(s, p12); OMG_STRING_ADD(s, p13); OMG_STRING_ADD(s, p14); OMG_STRING_ADD(s, p15); OMG_STRING_ADD(s, p16); OMG_STRING_ADD(s, p17); } while(0)
#define _OMG_STRING_ADD18(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); OMG_STRING_ADD(s, p12); OMG_STRING_ADD(s, p13); OMG_STRING_ADD(s, p14); OMG_STRING_ADD(s, p15); OMG_STRING_ADD(s, p16); OMG_STRING_ADD(s, p17); OMG_STRING_ADD(s, p18); } while(0)
#define _OMG_STRING_ADD19(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); OMG_STRING_ADD(s, p12); OMG_STRING_ADD(s, p13); OMG_STRING_ADD(s, p14); OMG_STRING_ADD(s, p15); OMG_STRING_ADD(s, p16); OMG_STRING_ADD(s, p17); OMG_STRING_ADD(s, p18); OMG_STRING_ADD(s, p19); } while(0)
#define _OMG_STRING_ADD20(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); OMG_STRING_ADD(s, p12); OMG_STRING_ADD(s, p13); OMG_STRING_ADD(s, p14); OMG_STRING_ADD(s, p15); OMG_STRING_ADD(s, p16); OMG_STRING_ADD(s, p17); OMG_STRING_ADD(s, p18); OMG_STRING_ADD(s, p19); OMG_STRING_ADD(s, p20); } while(0)
#define _OMG_STRING_ADD21(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); OMG_STRING_ADD(s, p12); OMG_STRING_ADD(s, p13); OMG_STRING_ADD(s, p14); OMG_STRING_ADD(s, p15); OMG_STRING_ADD(s, p16); OMG_STRING_ADD(s, p17); OMG_STRING_ADD(s, p18); OMG_STRING_ADD(s, p19); OMG_STRING_ADD(s, p20); OMG_STRING_ADD(s, p21); } while(0)
#define _OMG_STRING_ADD22(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); OMG_STRING_ADD(s, p12); OMG_STRING_ADD(s, p13); OMG_STRING_ADD(s, p14); OMG_STRING_ADD(s, p15); OMG_STRING_ADD(s, p16); OMG_STRING_ADD(s, p17); OMG_STRING_ADD(s, p18); OMG_STRING_ADD(s, p19); OMG_STRING_ADD(s, p20); OMG_STRING_ADD(s, p21); OMG_STRING_ADD(s, p22); } while(0)
#define _OMG_STRING_ADD23(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); OMG_STRING_ADD(s, p12); OMG_STRING_ADD(s, p13); OMG_STRING_ADD(s, p14); OMG_STRING_ADD(s, p15); OMG_STRING_ADD(s, p16); OMG_STRING_ADD(s, p17); OMG_STRING_ADD(s, p18); OMG_STRING_ADD(s, p19); OMG_STRING_ADD(s, p20); OMG_STRING_ADD(s, p21); OMG_STRING_ADD(s, p22); OMG_STRING_ADD(s, p23); } while(0)
#define _OMG_STRING_ADD24(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); OMG_STRING_ADD(s, p12); OMG_STRING_ADD(s, p13); OMG_STRING_ADD(s, p14); OMG_STRING_ADD(s, p15); OMG_STRING_ADD(s, p16); OMG_STRING_ADD(s, p17); OMG_STRING_ADD(s, p18); OMG_STRING_ADD(s, p19); OMG_STRING_ADD(s, p20); OMG_STRING_ADD(s, p21); OMG_STRING_ADD(s, p22); OMG_STRING_ADD(s, p23); OMG_STRING_ADD(s, p24); } while(0)
#define _OMG_STRING_ADD25(s, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25) do { OMG_STRING_ADD(s, p1); OMG_STRING_ADD(s, p2); OMG_STRING_ADD(s, p3); OMG_STRING_ADD(s, p4); OMG_STRING_ADD(s, p5); OMG_STRING_ADD(s, p6); OMG_STRING_ADD(s, p7); OMG_STRING_ADD(s, p8); OMG_STRING_ADD(s, p9); OMG_STRING_ADD(s, p10); OMG_STRING_ADD(s, p11); OMG_STRING_ADD(s, p12); OMG_STRING_ADD(s, p13); OMG_STRING_ADD(s, p14); OMG_STRING_ADD(s, p15); OMG_STRING_ADD(s, p16); OMG_STRING_ADD(s, p17); OMG_STRING_ADD(s, p18); OMG_STRING_ADD(s, p19); OMG_STRING_ADD(s, p20); OMG_STRING_ADD(s, p21); OMG_STRING_ADD(s, p22); OMG_STRING_ADD(s, p23); OMG_STRING_ADD(s, p24); OMG_STRING_ADD(s, p25); } while(0)
#define _OMG_STRING_ADD_INTERNAL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, NAME, ...) NAME
#define OMG_STRING_ADD_MANY(string, ...) _OMG_STRING_ADD_INTERNAL(__VA_ARGS__, _OMG_STRING_ADD25, _OMG_STRING_ADD24, _OMG_STRING_ADD23, _OMG_STRING_ADD22, _OMG_STRING_ADD21, _OMG_STRING_ADD20, _OMG_STRING_ADD19, _OMG_STRING_ADD18, _OMG_STRING_ADD17, _OMG_STRING_ADD16, _OMG_STRING_ADD15, _OMG_STRING_ADD14, _OMG_STRING_ADD13, _OMG_STRING_ADD12, _OMG_STRING_ADD11, _OMG_STRING_ADD10, _OMG_STRING_ADD9, _OMG_STRING_ADD8, _OMG_STRING_ADD7, _OMG_STRING_ADD6, _OMG_STRING_ADD5, _OMG_STRING_ADD4, _OMG_STRING_ADD3, _OMG_STRING_ADD2, _OMG_STRING_ADD1) (string, __VA_ARGS__)

#define _OMG_PRINT_BY_FUNC(omg, print_func, ...) do { \
    OMG_String _omg_temp_str; \
    if (omg_string_init_dynamic(&_omg_temp_str, NULL)) \
        break; \
    OMG_STRING_ADD_MANY(&_omg_temp_str, __VA_ARGS__); \
    print_func(omg, &_omg_temp_str); \
    omg_string_destroy(&_omg_temp_str); \
} while (0)
#if OMG_LOG_MIN_LEVEL <= OMG_LOG_CATEGORY_INFO
#define OMG_INFO(omg, ...) _OMG_PRINT_BY_FUNC(omg, omg->log_info_str, __VA_ARGS__)
#else
#define OMG_INFO(omg, ...) _OMG_UNUSED1(omg)
#endif

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

#endif

typedef struct {
    void* (*memset)(void *dest, register int val, register size_t len);
    void* (*memcpy)(void *dest, const void* src, size_t len);
    size_t (*strlen)(const char* src);
    void* memory_allocator;
    void* extra;
} OMG_Std;

typedef struct {
    size_t size;
    size_t len;
    int type;
    char* ptr;
    char buf[];
} OMG_String;

OMG_API void omg_std_set_default_handle(OMG_Std* this);
OMG_API void omg_std_fill_defaults(OMG_Std* this);
OMG_API size_t omg_strlen(const char* src);
OMG_API bool omg_string_init_dynamic(OMG_String* this, const OMG_String* base);
OMG_API bool omg_string_buffer_set_size(OMG_String* this, size_t new_size);
OMG_API bool omg_string_buffer_add_chunk(OMG_String* this);
OMG_API bool omg_string_resize(OMG_String* this, size_t new_len);
OMG_API bool omg_string_ensure_null(OMG_String* this);
OMG_API bool omg_string_add(OMG_String* this, const OMG_String* new_str);
OMG_API bool omg_string_destroy(OMG_String* this);
OMG_API bool omg_string_add_char(OMG_String* this, const char char_to_add);
OMG_API bool omg_string_add_char_p(OMG_String* this, const char* str_to_add);
