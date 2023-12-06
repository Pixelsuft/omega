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

#if OMG_WIN_PRECISE_ENCODING
#define _OMG_WIN_GET_ENCODE_SIZE(res, str, k32) do { \
    int _omg_temp_count = k32->MultiByteToWideChar(WIN_CP_UTF8, 0, str->ptr, str->len, NULL, 0); \
    res = (_omg_temp_count > 0) ? (size_t)(_omg_temp_count) : 0; \
} while (0)
#define _OMG_WIN_GET_DECODE_SIZE(res, str, k32, n_len) do { \
    int _omg_temp_count = k32->WideCharToMultiByte(WIN_CP_UTF8, 0, str, n_len, NULL, 0, NULL, NULL); \
    res = (_omg_temp_count > 0) ? (size_t)(_omg_temp_count) : 0; \
} while (0)
#else
#define _OMG_WIN_GET_ENCODE_SIZE(res, str, k32) do { res = ((size_t)(str->len)); } while (0)
#define _OMG_WIN_GET_DECODE_SIZE(res, str, k32, n_len) do { res = ((size_t)(n_len) * 4); } while (0)
#endif

#define _OMG_WIN_FORMAT_ERROR(mem, k32, error, buffer, len_buf) do { \
    buffer = _OMG_INTERNAL_MALLOC(mem, 128 * 1024); \
    if (OMG_ISNULL(buffer)) { \
        len_buf = 0; \
        break; \
    } \
    len_buf = k32->FormatMessageW( \
        OMG_WIN_FORMAT_MESSAGE_FROM_SYSTEM | OMG_WIN_FORMAT_MESSAGE_FROM_SYSTEM, \
        NULL, error, 0, buffer, 32 * 1024, NULL \
    ); \
} while (0)

#ifdef LOAD_IGNORE_CODE_AUTHZ_LEVEL
#define WIN_LOAD_IGNORE_CODE_AUTHZ_LEVEL LOAD_IGNORE_CODE_AUTHZ_LEVEL
#else
#define WIN_LOAD_IGNORE_CODE_AUTHZ_LEVEL 0x10
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
OMG_API void* omg_win_std_lib_load(const OMG_String* fn);
OMG_API void* omg_win_std_lib_func(void* lib, const OMG_String* func_name);
OMG_API bool omg_win_std_lib_free(void* lib);
#endif
