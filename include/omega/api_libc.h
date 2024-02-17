#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_SPNG
#include <omega/ostd.h>
#if !OMG_LIBC_DYNAMIC
#define OMG_LIBC_STD_PREFIX
typedef void FILE;
#else
#define OMG_LIBC_STD_PREFIX
#include <stdio.h>
#endif

typedef struct {
    void* handle;
    unsigned int OMG_LIBC_STD_PREFIX (*sleep)(unsigned int);
    int (*fclose)(FILE*);
    long (*ftell)(FILE*);
    int (*fseek)(FILE*, long, int);
    size_t (*fread)(void*, size_t, size_t, FILE*);
    size_t (*fwrite)(const void*, size_t, size_t, FILE*);
    FILE* (*fopen)(const char*, const char*);
    bool loaded;
} OMG_Libc;

OMG_API bool omg_libc_dll_load(OMG_Libc* this, const OMG_String* dll_path);
OMG_API bool omg_libc_dll_free(OMG_Libc* this);
#endif
