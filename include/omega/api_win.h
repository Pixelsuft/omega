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
    int _omg_temp_count = k32->MultiByteToWideChar(CP_UTF8, 0, str->ptr, str->len, NULL, 0); \
    res = (_omg_temp_count > 0) ? (size_t)(_omg_temp_count) : 0; \
} while (0)
#define _OMG_WIN_GET_DECODE_SIZE(res, str, k32, n_len) do { \
    int _omg_temp_count = k32->WideCharToMultiByte(CP_UTF8, 0, str, n_len, NULL, 0, NULL, NULL); \
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
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_SYSTEM, \
        NULL, error, 0, buffer, 32 * 1024, NULL \
    ); \
} while (0)

#ifndef MAKEINTRESOURCEA
#define MAKEINTRESOURCEA(res_to_make) ((LPSTR)((ULONG_PTR)((WORD)(res_to_make))))
#endif
#ifndef MAKEINTRESOURCEW
#define MAKEINTRESOURCEW(res_to_make) ((LPWSTR)((ULONG_PTR)((WORD)(res_to_make))))
#endif
#ifndef LOAD_IGNORE_CODE_AUTHZ_LEVEL
#define LOAD_IGNORE_CODE_AUTHZ_LEVEL 0x10
#endif
#ifndef FORMAT_MESSAGE_FROM_SYSTEM
#define FORMAT_MESSAGE_FROM_SYSTEM 0x00001000
#endif
#ifndef FORMAT_MESSAGE_FROM_SYSTEM
#define FORMAT_MESSAGE_FROM_SYSTEM 0x00001000
#endif
#ifndef STDOUT_HANDLE
#define STDOUT_HANDLE ((DWORD)-11)
#endif
#ifndef STDERR_HANDLE
#define STDERR_HANDLE ((DWORD)-12)
#endif
#ifndef CP_UTF8
#define CP_UTF8 65001
#endif
#ifndef ATTACH_PARENT_PROCESS
#define ATTACH_PARENT_PROCESS ((DWORD)-1)
#endif
#ifndef ERROR_ACCESS_DENIED
#define ERROR_ACCESS_DENIED 0x5
#endif
#ifndef ERROR_INVALID_HANDLE
#define ERROR_INVALID_HANDLE 0x6
#endif
#ifndef ERROR_GEN_FAILURE
#define ERROR_GEN_FAILURE 0x1F
#endif
#ifndef IMAGE_CURSOR
#define IMAGE_CURSOR 2
#endif
#ifndef LR_DEFAULTSIZE
#define LR_DEFAULTSIZE 0x0040
#endif
#ifndef LR_SHARED
#define LR_SHARED 0x8000
#endif
#ifndef CS_HREDRAW
#define CS_HREDRAW 0x0002
#endif
#ifndef CS_VREDRAW
#define CS_VREDRAW 0x0001
#endif
#ifndef IDC_ARROW
#define IDC_ARROW MAKEINTRESOURCEW(32512)
#endif
#ifndef SW_HIDE
#define SW_HIDE 0
#endif
#ifndef SW_SHOWNORMAL
#define SW_SHOWNORMAL 1
#endif
#ifndef SW_SHOWMINIMIZED
#define SW_SHOWMINIMIZED 2
#endif
#ifndef SW_SHOWMAXIMIZE
#define SW_SHOWMAXIMIZE 3
#endif
#ifndef SW_MINIMIZE
#define SW_MINIMIZE 6
#endif
#ifndef SW_RESTORE
#define SW_RESTORE 9
#endif
#ifndef SWP_NOSIZE
#define SWP_NOSIZE 0x0001
#endif
#ifndef SWP_NOMOVE
#define SWP_NOMOVE 0x0002
#endif
#ifndef SWP_FRAMECHANGED
#define SWP_FRAMECHANGED 0x020
#endif
#ifndef SWP_NOZORDER
#define SWP_NOZORDER 0x0004
#endif
#ifndef SWP_NOACTIVATE
#define SWP_NOACTIVATE 0x0010
#endif
#ifndef WS_EX_COMPOSITED
#define WS_EX_COMPOSITED 0x02000000
#endif
#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED 0x00080000
#endif
#ifndef WS_EX_NOINHERITLAYOUT
#define WS_EX_NOINHERITLAYOUT 0x00100000
#endif
#ifndef CW_USEDEFAULT
#define CW_USEDEFAULT ((int)0x80000000)
#endif
#ifndef WS_OVERLAPPED
#define WS_OVERLAPPED 0x00000000L
#endif
#ifndef WS_CAPTION
#define WS_CAPTION 0x00C00000L
#endif
#ifndef WS_CAPTION
#define WS_CAPTION 0x00C00000L
#endif
#ifndef WS_SYSMENU
#define WS_SYSMENU 0x00080000L
#endif
#ifndef WS_THICKFRAME
#define WS_THICKFRAME 0x00040000L
#endif
#ifndef WS_MINIMIZEBOX
#define WS_MINIMIZEBOX 0x00020000L
#endif
#ifndef WS_MAXIMIZEBOX
#define WS_MAXIMIZEBOX 0x00010000L
#endif
#ifndef WS_POPUP
#define WS_POPUP 0x80000000L
#endif
#ifndef WS_BORDER
#define WS_BORDER 0x00800000L
#endif
#ifndef WS_MAXIMIZE
#define WS_MAXIMIZE 0x01000000L
#endif
#ifndef WS_MINIMIZE
#define WS_MINIMIZE 0x20000000L
#endif
#ifndef WS_OVERLAPPEDWINDOW
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
#endif
#ifndef GWL_STYLE
#define GWL_STYLE (-16)
#endif
#ifndef GWLP_USERDATA
#define GWLP_USERDATA (-21)
#endif
#ifndef PM_REMOVE
#define PM_REMOVE 0x0001
#endif
#ifndef LOGPIXELSX
#define LOGPIXELSX 88
#endif
#ifndef LOGPIXELSY
#define LOGPIXELSY 90
#endif
#ifndef WM_NCCREATE
#define WM_NCCREATE 0x0081
#endif
#ifndef WM_THEMECHANGED
#define WM_THEMECHANGED 0x031A
#endif
#ifndef WM_DESTROY
#define WM_DESTROY 0x0002
#endif
#ifndef WM_PAINT
#define WM_PAINT 0x000F
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
    void OMG_WIN_STD_PREFIX (*ExitProcess)(UINT);
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
    HWND OMG_WIN_STD_PREFIX (*GetDesktopWindow)(void);
    BOOL OMG_WIN_STD_PREFIX (*GetWindowRect)(HWND, LPRECT);
    BOOL OMG_WIN_STD_PREFIX (*GetClientRect)(HWND, LPRECT);
    BOOL OMG_WIN_STD_PREFIX (*SetWindowPos)(HWND, HWND, int, int, int, int, UINT);
    BOOL OMG_WIN_STD_PREFIX (*MoveWindow)(HWND, int, int, int, int, BOOL);
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
    int OMG_WIN_STD_PREFIX (*GetSystemMetrics)(int);
    int OMG_WIN_STD_PREFIX (*GetSystemMetricsForDpi)(int, UINT);
    int OMG_WIN_STD_PREFIX (*GetDeviceCaps)(HDC, int);
    HDC OMG_WIN_STD_PREFIX (*GetDC)(HWND);
    HDC OMG_WIN_STD_PREFIX (*GetWindowDC)(HWND);
    int OMG_WIN_STD_PREFIX (*ReleaseDC)(HWND, HDC);
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
