#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_SPNG
#include <omega/ostd.h>
#if !OMG_LIBC_DYNAMIC
#define OMG_LIBC_STD_PREFIX
#else
#define OMG_LIBC_STD_PREFIX
#endif

typedef struct {
    void* handle;
    bool loaded;
} OMG_Libc;

OMG_API bool omg_libc_dll_load(OMG_Libc* this, const OMG_String* dll_path);
OMG_API bool omg_libc_dll_free(OMG_Libc* this);
#endif
