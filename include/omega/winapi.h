#pragma once
#include <omega/config.h>
#include <omega/ostd.h>
#if OMG_IS_WIN
#define WIN32_LEAN_AND_MEAN
#if OMG_WINAPI_DYNAMIC
#include <libloaderapi.h>
#include <basetsd.h>
#else
#include <windows.h>
#endif

typedef struct {
    HMODULE handle;
    HMODULE (*LoadLibraryExW)(LPCWSTR, HANDLE, DWORD);
    BOOL (*FreeLibrary)(HMODULE);
    HANDLE (*HeapCreate)(DWORD, SIZE_T, SIZE_T);
    BOOL (*HeapDestroy)(HANDLE);
    LPVOID (*HeapAlloc)(HANDLE, DWORD, SIZE_T);
    BOOL (*HeapFree)(HANDLE, DWORD, LPVOID);
} OMG_Kernel32;

OMG_API bool omg_kernel32_load(OMG_Kernel32* this);
OMG_API bool omg_kernel32_free(OMG_Kernel32* this);
#endif
