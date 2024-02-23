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
#include <sys/stat.h>
#include <pthread.h>
#if OMG_IS_UNIX
#include <unistd.h>
#endif
#endif

#ifndef S_ISDIR
#define S_ISDIR(mode) ((((mode)) & 0170000) == (0040000))
#endif
#ifndef PTHREAD_CREATE_JOINABLE
#define PTHREAD_CREATE_JOINABLE 0x00000000
#endif
#ifndef PTHREAD_CANCEL_ASYNCHRONOUS
#define PTHREAD_CANCEL_ASYNCHRONOUS 1
#endif

typedef struct {
    void* handle;
    unsigned int OMG_LIBC_STD_PREFIX (*sleep)(unsigned int);
    int OMG_LIBC_STD_PREFIX (*fclose)(FILE*);
    long OMG_LIBC_STD_PREFIX (*ftell)(FILE*);
    int OMG_LIBC_STD_PREFIX (*fseek)(FILE*, long, int);
    size_t OMG_LIBC_STD_PREFIX (*fread)(void*, size_t, size_t, FILE*);
    size_t OMG_LIBC_STD_PREFIX (*fwrite)(const void*, size_t, size_t, FILE*);
    FILE* OMG_LIBC_STD_PREFIX (*fopen)(const char*, const char*);
    int OMG_LIBC_STD_PREFIX (*access)(const char*, int);
    int OMG_LIBC_STD_PREFIX (*stat)(const char*, struct stat*);
    int OMG_LIBC_STD_PREFIX (*remove)(const char*);
    int OMG_LIBC_STD_PREFIX (*rmdir)(const char*);
    int OMG_LIBC_STD_PREFIX (*rename)(const char*, const char*);
    int OMG_LIBC_STD_PREFIX (*pthread_attr_init)(pthread_attr_t*);
    int OMG_LIBC_STD_PREFIX (*pthread_attr_setdetachstate)(pthread_attr_t*, int);
    int OMG_LIBC_STD_PREFIX (*pthread_attr_setstacksize)(pthread_attr_t*, size_t);
    int OMG_LIBC_STD_PREFIX (*pthread_create)(pthread_t*, const pthread_attr_t*, void *(*)(void *), void*);
    int OMG_LIBC_STD_PREFIX (*pthread_setcanceltype)(int, int*);
    pthread_t OMG_LIBC_STD_PREFIX (*pthread_self)(void);
    int OMG_LIBC_STD_PREFIX (*pthread_getschedparam)(pthread_t, int*, struct sched_param*);
    int OMG_LIBC_STD_PREFIX (*pthread_setschedparam)(pthread_t, int, const struct sched_param*);
    bool loaded;
} OMG_Libc;

OMG_API bool omg_libc_dll_load(OMG_Libc* this, const OMG_String* dll_path);
OMG_API bool omg_libc_dll_free(OMG_Libc* this);
#endif
