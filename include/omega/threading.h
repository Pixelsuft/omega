#pragma once
#include <omega/config.h>
#if OMG_IS_WIN && OMG_SUPPORT_THREADING
#include <process.h>
// uintptr_t _beginthreadex(void *_Security, unsigned int _StackSize, unsigned int (*_StartAddress)(void *), void *_ArgList, unsigned int _InitFlag, unsigned int *_ThrdAddr);
// void _endthreadex(unsigned int _Retval);
#ifndef OMG_beginthread
#define OMG_beginthread _beginthreadex
#endif
#ifndef OMG_endthread
#define OMG_endthread _endthreadex
#endif
#define OMG_THREAD_RESERVED1 ((void*)((size_t)OMG_beginthread))
#define OMG_THREAD_RESERVED2 ((void*)((size_t)OMG_endthread))
#else
#define OMG_THREAD_RESERVED1 NULL
#define OMG_THREAD_RESERVED2 NULL
#endif

typedef int (*OMG_ThreadFunction)(void* data);

typedef struct {
    int dummy;
} OMG_Thread;
