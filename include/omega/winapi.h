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

#ifdef FORMAT_MESSAGE_FROM_SYSTEM
#define OMG_WIN_FORMAT_MESSAGE_FROM_SYSTEM FORMAT_MESSAGE_FROM_SYSTEM
#else
#define OMG_WIN_FORMAT_MESSAGE_FROM_SYSTEM 0x00001000
#endif
#ifdef FORMAT_MESSAGE_FROM_SYSTEM
#define OMG_WIN_FORMAT_MESSAGE_FROM_SYSTEM FORMAT_MESSAGE_FROM_SYSTEM
#else
#define OMG_WIN_FORMAT_MESSAGE_FROM_SYSTEM 0x00001000
#endif
#ifdef STD_OUTPUT_HANDLE
#define WIN_STD_OUTPUT_HANDLE STD_OUTPUT_HANDLE
#else
#define WIN_STD_OUTPUT_HANDLE ((DWORD)-11)
#endif
#ifdef STD_ERROR_HANDLE
#define WIN_STD_ERROR_HANDLE STD_ERROR_HANDLE
#else
#define WIN_STD_ERROR_HANDLE ((DWORD)-12)
#endif
#ifdef CP_UTF8
#define WIN_CP_UTF8 CP_UTF8
#else
#define WIN_CP_UTF8 65001
#endif
#ifdef ATTACH_PARENT_PROCESS
#define WIN_ATTACH_PARENT_PROCESS ATTACH_PARENT_PROCESS
#else
#define WIN_ATTACH_PARENT_PROCESS ((DWORD)-1)
#endif
#ifdef ERROR_ACCESS_DENIED
#define WIN_ERROR_ACCESS_DENIED ERROR_ACCESS_DENIED
#else
#define WIN_ERROR_ACCESS_DENIED 0x5
#endif
#ifdef ERROR_INVALID_HANDLE
#define WIN_ERROR_INVALID_HANDLE ERROR_INVALID_HANDLE
#else
#define WIN_ERROR_INVALID_HANDLE 0x6
#endif
#ifdef ERROR_GEN_FAILURE
#define WIN_ERROR_GEN_FAILURE ERROR_GEN_FAILURE
#else
#define WIN_ERROR_GEN_FAILURE 0x1F
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
    FARPROC (*GetProcAddress)(HMODULE, LPCSTR);
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
    int (*WideCharToMultiByte)(UINT, DWORD, LPCWCH, int, LPSTR, int, LPCCH, LPBOOL);
    BOOL WINAPI (*WriteConsoleW)(HANDLE, const VOID*, DWORD, LPDWORD, LPVOID);
    DWORD (*FormatMessageW)(DWORD, LPCVOID, DWORD, DWORD, LPWSTR, DWORD, void* args);
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
