#pragma once
#include <omega/config.h>
#include <omega/ostd.h>
#if OMG_IS_WIN
#define WIN32_LEAN_AND_MEAN
#if OMG_WINAPI_DYNAMIC
#include <libloaderapi.h>
#include <basetsd.h>
#include <windows.h>
#else
#include <windows.h>
#endif
#define OMG_WIN_STD_PREFIX WINAPI

#if OMG_WIN_PRECISE_ENCODING
#define _OMG_WIN_GET_ENCODE_SIZE(res, str, k32) do { \
    int _omg_temp_count = k32->MultiByteToWideChar(OMG_WIN_CP_UTF8, 0, str->ptr, str->len, NULL, 0); \
    res = (_omg_temp_count > 0) ? (size_t)(_omg_temp_count) : 0; \
} while (0)
#define _OMG_WIN_GET_DECODE_SIZE(res, str, k32, n_len) do { \
    int _omg_temp_count = k32->WideCharToMultiByte(OMG_WIN_CP_UTF8, 0, str, n_len, NULL, 0, NULL, NULL); \
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
#define OMG_WIN_LOAD_IGNORE_CODE_AUTHZ_LEVEL LOAD_IGNORE_CODE_AUTHZ_LEVEL
#else
#define OMG_WIN_LOAD_IGNORE_CODE_AUTHZ_LEVEL 0x10
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
#define OMG_WIN_STD_OUTPUT_HANDLE STD_OUTPUT_HANDLE
#else
#define OMG_WIN_STD_OUTPUT_HANDLE ((DWORD)-11)
#endif
#ifdef STD_ERROR_HANDLE
#define OMG_WIN_STD_ERROR_HANDLE STD_ERROR_HANDLE
#else
#define OMG_WIN_STD_ERROR_HANDLE ((DWORD)-12)
#endif
#ifdef CP_UTF8
#define OMG_WIN_CP_UTF8 CP_UTF8
#else
#define OMG_WIN_CP_UTF8 65001
#endif
#ifdef ATTACH_PARENT_PROCESS
#define OMG_WIN_ATTACH_PARENT_PROCESS ATTACH_PARENT_PROCESS
#else
#define OMG_WIN_ATTACH_PARENT_PROCESS ((DWORD)-1)
#endif
#ifdef ERROR_ACCESS_DENIED
#define OMG_WIN_ERROR_ACCESS_DENIED ERROR_ACCESS_DENIED
#else
#define OMG_WIN_ERROR_ACCESS_DENIED 0x5
#endif
#ifdef ERROR_INVALID_HANDLE
#define OMG_WIN_ERROR_INVALID_HANDLE ERROR_INVALID_HANDLE
#else
#define OMG_WIN_ERROR_INVALID_HANDLE 0x6
#endif
#ifdef ERROR_GEN_FAILURE
#define OMG_WIN_ERROR_GEN_FAILURE ERROR_GEN_FAILURE
#else
#define OMG_WIN_ERROR_GEN_FAILURE 0x1F
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
} OMG_NTDLL_OSVERSIONINFOEXW;

typedef enum
{
    Default,
    AllowDark,
    ForceDark,
    ForceLight,
    Max
} OMG_WinPreferredAppMode;

#if !OMG_WINAPI_DYNAMIC && !OMG_WINAPI_DYNAMIC_UGLY
OMG_C_EXPORT __declspec(dllimport) void RtlGetVersion(NTDLL_OSVERSIONINFOEXW* version_information);
#endif

typedef struct {
    HMODULE handle;
    HMODULE OMG_WIN_STD_PREFIX (*LoadLibraryExW)(LPCWSTR, HANDLE, DWORD);
    BOOL OMG_WIN_STD_PREFIX (*FreeLibrary)(HMODULE);
    FARPROC OMG_WIN_STD_PREFIX (*GetProcAddress)(HMODULE, LPCSTR);
    HANDLE OMG_WIN_STD_PREFIX (*HeapCreate)(DWORD, SIZE_T, SIZE_T);
    BOOL OMG_WIN_STD_PREFIX (*HeapDestroy)(HANDLE);
    LPVOID OMG_WIN_STD_PREFIX (*HeapAlloc)(HANDLE, DWORD, SIZE_T);
    LPVOID OMG_WIN_STD_PREFIX (*HeapReAlloc)(HANDLE, DWORD, LPVOID, SIZE_T);
    BOOL OMG_WIN_STD_PREFIX (*HeapFree)(HANDLE, DWORD, LPVOID);
    DWORD OMG_WIN_STD_PREFIX (*GetLastError)(void);
    HANDLE OMG_WIN_STD_PREFIX (*GetStdHandle)(DWORD);
    BOOL OMG_WIN_STD_PREFIX (*AllocConsole)(void);
    BOOL OMG_WIN_STD_PREFIX (*FreeConsole)(void);
    BOOL OMG_WIN_STD_PREFIX (*AttachConsole)(DWORD);
    int OMG_WIN_STD_PREFIX (*MultiByteToWideChar)(UINT, DWORD, const char*, int, LPWSTR, int);
    int OMG_WIN_STD_PREFIX (*WideCharToMultiByte)(UINT, DWORD, LPCWCH, int, LPSTR, int, LPCCH, LPBOOL);
    BOOL OMG_WIN_STD_PREFIX (*WriteConsoleW)(HANDLE, const VOID*, DWORD, LPDWORD, LPVOID);
    DWORD OMG_WIN_STD_PREFIX (*FormatMessageW)(DWORD, LPCVOID, DWORD, DWORD, LPWSTR, DWORD, char** args);
} OMG_Kernel32;

typedef struct {
    HANDLE handle;
    void OMG_WIN_STD_PREFIX (*RtlGetVersion)(OMG_NTDLL_OSVERSIONINFOEXW*);
} OMG_Ntdll;

typedef struct {
    HANDLE handle;
    BOOL OMG_WIN_STD_PREFIX (*SetProcessDPIAware)(void);
    UINT OMG_WIN_STD_PREFIX (*GetDpiForSystem)(void);
    UINT OMG_WIN_STD_PREFIX (*GetDpiForWindow)(HWND hwnd);
    int OMG_WIN_STD_PREFIX (*GetSystemMetricsForDpi)(int index, UINT dpi);
} OMG_User32;

typedef struct {
    HANDLE handle;
    HRESULT OMG_WIN_STD_PREFIX (*DwmSetWindowAttribute)(HWND, DWORD, LPCVOID, DWORD);
    HRESULT OMG_WIN_STD_PREFIX (*DwmFlush)(void);
} OMG_Dwmapi;

typedef struct {
    HANDLE handle;
    bool OMG_WIN_STD_PREFIX (*ShouldAppsUseDarkMode)(void);
    void OMG_WIN_STD_PREFIX (*AllowDarkModeForWindow)(HWND hwnd, bool allow);
    void OMG_WIN_STD_PREFIX (*AllowDarkModeForApp)(bool allow);
    void OMG_WIN_STD_PREFIX (*FlushMenuThemes)(void);
    void OMG_WIN_STD_PREFIX (*RefreshImmersiveColorPolicyState)(void);
    bool OMG_WIN_STD_PREFIX (*IsDarkModeAllowedForWindow)(HWND hwnd);
    bool OMG_WIN_STD_PREFIX (*ShouldSystemUseDarkMode)(void);
    OMG_WinPreferredAppMode OMG_WIN_STD_PREFIX (*SetPreferredAppMode)(OMG_WinPreferredAppMode app_mode);
    bool OMG_WIN_STD_PREFIX (*IsDarkModeAllowedForApp)(void);
} OMG_Uxtheme;

OMG_API bool omg_winapi_kernel32_load(OMG_Kernel32* this);
OMG_API bool omg_winapi_kernel32_free(OMG_Kernel32* this);
OMG_API bool omg_winapi_user32_load(OMG_User32* this);
OMG_API bool omg_winapi_user32_free(OMG_User32* this);
OMG_API bool omg_winapi_ntdll_load(OMG_Ntdll* this);
OMG_API bool omg_winapi_ntdll_free(OMG_Ntdll* this);
OMG_API bool omg_winapi_dwmapi_load(OMG_Dwmapi* this);
OMG_API bool omg_winapi_dwmapi_free(OMG_Dwmapi* this);
OMG_API bool omg_winapi_uxtheme_load(OMG_Uxtheme* this);
OMG_API bool omg_winapi_uxtheme_free(OMG_Uxtheme* this);
#endif
