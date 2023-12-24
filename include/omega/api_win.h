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

#ifdef MAKEINTRESOURCEA
#define OMG_WIN_MAKEINTRESOURCEA MAKEINTRESOURCEA
#else
#define OMG_WIN_MAKEINTRESOURCEA(res_to_make) ((LPSTR)((ULONG_PTR)((WORD)(res_to_make))))
#endif
#ifdef MAKEINTRESOURCEW
#define OMG_WIN_MAKEINTRESOURCEW MAKEINTRESOURCEW
#else
#define OMG_WIN_MAKEINTRESOURCEW(res_to_make) ((LPWSTR)((ULONG_PTR)((WORD)(res_to_make))))
#endif
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
#ifdef IMAGE_CURSOR
#define OMG_WIN_IMAGE_CURSOR IMAGE_CURSOR
#else
#define OMG_WIN_IMAGE_CURSOR 2
#endif
#ifdef LR_DEFAULTSIZE
#define OMG_WIN_LR_DEFAULTSIZE LR_DEFAULTSIZE
#else
#define OMG_WIN_LR_DEFAULTSIZE 0x0040
#endif
#ifdef LR_SHARED
#define OMG_WIN_LR_SHARED LR_SHARED
#else
#define OMG_WIN_LR_SHARED 0x8000
#endif
#ifdef CS_HREDRAW
#define OMG_WIN_CS_HREDRAW CS_HREDRAW
#else
#define OMG_WIN_CS_HREDRAW 0x0002
#endif
#ifdef CS_VREDRAW
#define OMG_WIN_CS_VREDRAW CS_VREDRAW
#else
#define OMG_WIN_CS_VREDRAW 0x0001
#endif
#ifdef IDC_ARROW
#define OMG_WIN_IDC_ARROW IDC_ARROW
#else
#define OMG_WIN_IDC_ARROW OMG_WIN_MAKEINTRESOURCEW(32512)
#endif
#ifdef SW_HIDE
#define OMG_WIN_SW_HIDE SW_HIDE
#else
#define OMG_WIN_SW_HIDE 0
#endif
#ifdef SW_SHOWNORMAL
#define OMG_WIN_SW_SHOWNORMAL SW_SHOWNORMAL
#else
#define OMG_WIN_SW_SHOWNORMAL 1
#endif
#ifdef WS_EX_COMPOSITED
#define OMG_WIN_WS_EX_COMPOSITED WS_EX_COMPOSITED
#else
#define OMG_WIN_WS_EX_COMPOSITED 0x02000000
#endif
#ifdef WS_EX_LAYERED
#define OMG_WIN_WS_EX_LAYERED WS_EX_LAYERED
#else
#define OMG_WIN_WS_EX_LAYERED 0x00080000
#endif
#ifdef WS_EX_NOINHERITLAYOUT
#define OMG_WIN_WS_EX_NOINHERITLAYOUT WS_EX_NOINHERITLAYOUT
#else
#define OMG_WIN_WS_EX_NOINHERITLAYOUT 0x00100000
#endif
#ifdef CW_USEDEFAULT
#define OMG_WIN_CW_USEDEFAULT CW_USEDEFAULT
#else
#define OMG_WIN_CW_USEDEFAULT ((int)0x80000000)
#endif
#ifdef WS_OVERLAPPED
#define OMG_WIN_WS_OVERLAPPED WS_OVERLAPPED
#else
#define OMG_WIN_WS_OVERLAPPED 0x00000000
#endif
#ifdef WS_CAPTION
#define OMG_WIN_WS_CAPTION WS_CAPTION
#else
#define OMG_WIN_WS_CAPTION 0x00C00000
#endif
#ifdef WS_SYSMENU
#define OMG_WIN_WS_SYSMENU WS_SYSMENU
#else
#define OMG_WIN_WS_SYSMENU 0x00080000
#endif
#ifdef WS_THICKFRAME
#define OMG_WIN_WS_THICKFRAME WS_THICKFRAME
#else
#define OMG_WIN_WS_THICKFRAME 0x00040000
#endif
#ifdef WS_MINIMIZEBOX
#define OMG_WIN_WS_MINIMIZEBOX WS_MINIMIZEBOX
#else
#define OMG_WIN_WS_MINIMIZEBOX 0x00020000
#endif
#ifdef WS_MAXIMIZEBOX
#define OMG_WIN_WS_MAXIMIZEBOX WS_MAXIMIZEBOX
#else
#define OMG_WIN_WS_MAXIMIZEBOX 0x00010000
#endif
#ifdef WS_OVERLAPPEDWINDOW
#define OMG_WIN_WS_OVERLAPPEDWINDOW WS_OVERLAPPEDWINDOW
#else
#define OMG_WIN_WS_OVERLAPPEDWINDOW (OMG_WIN_WS_OVERLAPPED | OMG_WIN_WS_CAPTION | OMG_WIN_WS_SYSMENU | OMG_WIN_WS_THICKFRAME | OMG_WIN_WS_MINIMIZEBOX | OMG_WIN_WS_MAXIMIZEBOX)
#endif
#ifdef GWLP_USERDATA
#define OMG_WIN_GWLP_USERDATA GWLP_USERDATA
#else
#define OMG_WIN_GWLP_USERDATA (-21)
#endif
#ifdef PM_REMOVE
#define OMG_WIN_PM_REMOVE PM_REMOVE
#else
#define OMG_WIN_PM_REMOVE 0x0001
#endif
#ifdef WM_NCCREATE
#define OMG_WIN_WM_NCCREATE WM_NCCREATE
#else
#define OMG_WIN_WM_NCCREATE 0x0081
#endif
#ifdef WM_THEMECHANGED
#define OMG_WIN_WM_THEMECHANGED WM_THEMECHANGED
#else
#define OMG_WIN_WM_THEMECHANGED 0x031A
#endif
#ifdef WM_DESTROY
#define OMG_WIN_WM_DESTROY WM_DESTROY
#else
#define OMG_WIN_WM_DESTROY 0x0002
#endif

typedef WNDCLASSEXW OMG_WIN_WNDCLASSEXW;
typedef WNDPROC OMG_WIN_WNDPROC;
typedef LPCREATESTRUCTW OMG_WIN_LPCREATESTRUCTW;

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
} OMG_WIN_NTDLL_OSVERSIONINFOEXW;

typedef enum
{
    OMG_WIN_APPMODE_DEFAULT,
    OMG_WIN_APPMODE_ALLOW_DARK,
    OMG_WIN_APPMODE_FORCE_DARK,
    OMG_WIN_APPMODE_FORCE_LIGHT,
    OMG_WIN_APPMODE_MAX
} OMG_WinPreferredAppMode;

#if !OMG_WINAPI_DYNAMIC && !OMG_WINAPI_DYNAMIC_UGLY
OMG_C_EXPORT __declspec(dllimport) void RtlGetVersion(OMG_WIN_NTDLL_OSVERSIONINFOEXW* version_information);
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
    void OMG_WIN_STD_PREFIX (*Sleep)(DWORD);
    DWORD OMG_WIN_STD_PREFIX (*GetLastError)(void);
    HANDLE OMG_WIN_STD_PREFIX (*GetStdHandle)(DWORD);
    BOOL OMG_WIN_STD_PREFIX (*AllocConsole)(void);
    BOOL OMG_WIN_STD_PREFIX (*FreeConsole)(void);
    BOOL OMG_WIN_STD_PREFIX (*AttachConsole)(DWORD);
    int OMG_WIN_STD_PREFIX (*MultiByteToWideChar)(UINT, DWORD, const char*, int, LPWSTR, int);
    int OMG_WIN_STD_PREFIX (*WideCharToMultiByte)(UINT, DWORD, LPCWCH, int, LPSTR, int, LPCCH, LPBOOL);
    BOOL OMG_WIN_STD_PREFIX (*WriteConsoleW)(HANDLE, const VOID*, DWORD, LPDWORD, LPVOID);
    DWORD OMG_WIN_STD_PREFIX (*FormatMessageW)(DWORD, LPCVOID, DWORD, DWORD, LPWSTR, DWORD, char** args);
    HMODULE OMG_WIN_STD_PREFIX (*GetModuleHandleW)(LPCWSTR);
} OMG_Kernel32;

typedef struct {
    HANDLE handle;
    void OMG_WIN_STD_PREFIX (*RtlGetVersion)(OMG_WIN_NTDLL_OSVERSIONINFOEXW*);
} OMG_Ntdll;

#ifdef SetWindowLongW
// TODO: fuck microsoft
#undef SetWindowLongW
#endif

typedef struct {
    HANDLE handle;
    ATOM OMG_WIN_STD_PREFIX (*RegisterClassExW)(const WNDCLASSEXW*);
    BOOL OMG_WIN_STD_PREFIX (*UnregisterClassW)(LPCWSTR, HINSTANCE hInstance);
    HWND OMG_WIN_STD_PREFIX (*CreateWindowExW)(DWORD, LPCWSTR, LPCWSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
    BOOL OMG_WIN_STD_PREFIX (*DestroyWindow)(HWND);
    LONG OMG_WIN_STD_PREFIX (*SetWindowLongW)(HWND, int, LONG);
    LONG OMG_WIN_STD_PREFIX (*GetWindowLongW)(HWND, int);
#ifndef SetWindowLongPtrW
    LONG_PTR OMG_WIN_STD_PREFIX (*SetWindowLongPtrW)(HWND, int, LONG_PTR);
    LONG_PTR OMG_WIN_STD_PREFIX (*GetWindowLongPtrW)(HWND, int);
#endif
    BOOL OMG_WIN_STD_PREFIX (*ShowWindow)(HWND, INT);
    BOOL OMG_WIN_STD_PREFIX (*SetWindowTextW)(HWND, LPCWSTR);
    BOOL OMG_WIN_STD_PREFIX (*PeekMessageW)(LPMSG, HWND, UINT, UINT, UINT);
    BOOL OMG_WIN_STD_PREFIX (*TranslateMessage)(const MSG*);
    LRESULT OMG_WIN_STD_PREFIX (*DispatchMessageW)(const MSG*);
    BOOL OMG_WIN_STD_PREFIX (*InvalidateRect)(HWND, const RECT*, BOOL);
    BOOL OMG_WIN_STD_PREFIX (*UpdateWindow)(HWND);
    LRESULT OMG_WIN_STD_PREFIX (*DefWindowProcW)(HWND, UINT, WPARAM, LPARAM);
    HBRUSH OMG_WIN_STD_PREFIX (*GetSysColorBrush)(int);
    HANDLE OMG_WIN_STD_PREFIX (*LoadImageW)(HINSTANCE, LPCWSTR, UINT, int, int, UINT);
    BOOL OMG_WIN_STD_PREFIX (*SetProcessDPIAware)(void);
    UINT OMG_WIN_STD_PREFIX (*GetDpiForSystem)(void);
    UINT OMG_WIN_STD_PREFIX (*GetDpiForWindow)(HWND);
    int OMG_WIN_STD_PREFIX (*GetSystemMetricsForDpi)(int, UINT);
} OMG_User32;

typedef struct {
    HANDLE handle;
    HRESULT OMG_WIN_STD_PREFIX (*DwmSetWindowAttribute)(HWND, DWORD, LPCVOID, DWORD);
    HRESULT OMG_WIN_STD_PREFIX (*DwmFlush)(void);
} OMG_Dwmapi;

typedef struct {
    HANDLE handle;
    bool OMG_WIN_STD_PREFIX (*ShouldAppsUseDarkMode)(void);
    void OMG_WIN_STD_PREFIX (*AllowDarkModeForWindow)(HWND, bool);
    void OMG_WIN_STD_PREFIX (*AllowDarkModeForApp)(bool);
    void OMG_WIN_STD_PREFIX (*FlushMenuThemes)(void);
    void OMG_WIN_STD_PREFIX (*RefreshImmersiveColorPolicyState)(void);
    bool OMG_WIN_STD_PREFIX (*IsDarkModeAllowedForWindow)(HWND);
    bool OMG_WIN_STD_PREFIX (*ShouldSystemUseDarkMode)(void);
    OMG_WinPreferredAppMode OMG_WIN_STD_PREFIX (*SetPreferredAppMode)(OMG_WinPreferredAppMode);
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
OMG_API bool omg_winapi_uxtheme_load(OMG_Uxtheme* this, int build_num);
OMG_API bool omg_winapi_uxtheme_free(OMG_Uxtheme* this);
#endif
