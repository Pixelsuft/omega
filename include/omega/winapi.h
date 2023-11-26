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
    ULONG dwOSVersionInfoSize;
    ULONG dwMajorVersion;
    ULONG dwMinorVersion;
    ULONG dwBuildNumber;
    ULONG dwPlatformId;
    WCHAR szCSDVersion[128];
    USHORT wServicePackMajor;
    USHORT wServicePackMinor;
    USHORT wSuiteMask;
    UCHAR wProductType;
    UCHAR wReserved;
} NTDLL_OSVERSIONINFOEXW;

#if !OMG_WINAPI_DYNAMIC && !OMG_WINAPI_DYNAMIC_UGLY
OMG_C_EXPORT __declspec(dllimport) void RtlGetVersion(NTDLL_OSVERSIONINFOEXW* version_information);
#endif

typedef struct {
    HMODULE handle;
    HMODULE (*LoadLibraryExW)(LPCWSTR, HANDLE, DWORD);
    BOOL (*FreeLibrary)(HMODULE);
    HANDLE (*HeapCreate)(DWORD, SIZE_T, SIZE_T);
    BOOL (*HeapDestroy)(HANDLE);
    LPVOID (*HeapAlloc)(HANDLE, DWORD, SIZE_T);
    LPVOID (*HeapReAlloc)(HANDLE, DWORD, LPVOID, SIZE_T);
    BOOL (*HeapFree)(HANDLE, DWORD, LPVOID);
    DWORD (*GetLastError)(void);
    HANDLE WINAPI (*GetStdHandle)(DWORD);
    BOOL WINAPI (*AllocConsole)(void);
    BOOL WINAPI (*FreeConsole)(void);
    BOOL WINAPI (*AttachConsole)(DWORD);
    int (*MultiByteToWideChar)(UINT, DWORD, const char*, int, LPWSTR, int);
    BOOL WINAPI (*WriteConsoleW)(HANDLE, const VOID*, DWORD, LPDWORD, LPVOID);
} OMG_Kernel32;

typedef struct {
    HANDLE handle;
    void (*RtlGetVersion)(NTDLL_OSVERSIONINFOEXW*);
} OMG_Ntdll;

OMG_API bool omg_winapi_kernel32_load(OMG_Kernel32* this);
OMG_API bool omg_winapi_kernel32_free(OMG_Kernel32* this);
OMG_API bool omg_winapi_ntdll_load(OMG_Ntdll* this);
OMG_API bool omg_winapi_ntdll_free(OMG_Ntdll* this);
#endif
