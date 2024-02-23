#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_BEGIN_END_THREAD
#include <process.h>
// uintptr_t _beginthreadex(void *_Security, unsigned int _StackSize, unsigned int (*_StartAddress)(void *), void *_ArgList, unsigned int _InitFlag, unsigned int *_ThrdAddr);
// void _endthreadex(unsigned int _Retval);
#define OMG_THREAD_RESERVED1(omg) ((void*)((size_t)(((OMG_Msvcrt*)(omg->msvcrt))->_beginthreadex)))
#define OMG_THREAD_RESERVED2(omg) ((void*)((size_t)(((OMG_Msvcrt*)(omg->msvcrt))->_endthreadex)))
#else
#define OMG_THREAD_RESERVED1(omg) NULL
#define OMG_THREAD_RESERVED2(omg) NULL
#endif

#define OMG_THREAD_PRIORITY_LOW  0
#define OMG_THREAD_PRIORITY_NORMAL 1
#define OMG_THREAD_PRIORITY_HIGH 2
#define OMG_THREAD_PRIORITY_TIME_CRITICAL 3

#define OMG_THREAD_CREATE_DEF(thread, omg, func, name, data, stack_size) do { \
    thread = omg->thread_create( \
        omg, (OMG_ThreadFunction)(func), name, data, stack_size, \
        OMG_THREAD_RESERVED1(omg), OMG_THREAD_RESERVED2(omg) \
    ); \
} while (0)

#define OMG_THREAD_CREATE(thread, omg, func, name, data, stack_size) OMG_THREAD_CREATE_DEF(thread, omg, func, name, data, stack_size)

#if OMG_IS_WIN
typedef int (__stdcall *OMG_ThreadFunction)(void* data);
#else
typedef int (*OMG_ThreadFunction)(void* data);
#endif

typedef struct {
    int dummy;
} OMG_Thread;
