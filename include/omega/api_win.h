#pragma once
#include <omega/config.h>
#include <omega/ostd.h>
#if OMG_IS_WIN
#define WIN32_LEAN_AND_MEAN
#define OEMRESOURCE
#if OMG_WINAPI_DYNAMIC
#include <windows.h>
#include <libloaderapi.h>
#include <basetsd.h>
#else
#include <windows.h>
#include <windowsx.h>
#endif
#if OMG_IS_VC // Fuck Visual Studio
#define OMG_WIN_STD_PREFIX
#else
#define OMG_WIN_STD_PREFIX WINAPI
#endif

#if OMG_WIN_PRECISE_ENCODING
#define _OMG_WIN_GET_ENCODE_SIZE(res, str, k32) do { \
    int _omg_temp_count = k32->MultiByteToWideChar(CP_UTF8, 0, str->ptr, (int)str->len, NULL, 0); \
    res = (_omg_temp_count > 0) ? (size_t)(_omg_temp_count) : 0; \
} while (0)
#define _OMG_WIN_GET_DECODE_SIZE(res, str, k32, n_len) do { \
    int _omg_temp_count = k32->WideCharToMultiByte(CP_UTF8, 0, str, (int)n_len, NULL, 0, NULL, NULL); \
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

#ifndef DPI_AWARENESS_CONTEXT
#define DPI_AWARENESS_CONTEXT int64_t
#endif
#ifndef DPI_AWARENESS_CONTEXT_UNAWARE
#define DPI_AWARENESS_CONTEXT_UNAWARE              ((DPI_AWARENESS_CONTEXT)-1)
#endif
#ifndef DPI_AWARENESS_CONTEXT_SYSTEM_AWARE
#define DPI_AWARENESS_CONTEXT_SYSTEM_AWARE         ((DPI_AWARENESS_CONTEXT)-2)
#endif
#ifndef DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE    ((DPI_AWARENESS_CONTEXT)-3)
#endif
#ifndef DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((DPI_AWARENESS_CONTEXT)-4)
#endif
#ifndef DPI_AWARENESS_CONTEXT_UNAWARE_GDISCALED
#define DPI_AWARENESS_CONTEXT_UNAWARE_GDISCALED    ((DPI_AWARENESS_CONTEXT)-5)
#endif
#ifndef MAKEINTRESOURCEA
#define MAKEINTRESOURCEA(res_to_make) ((LPSTR)((ULONG_PTR)((WORD)(res_to_make))))
#endif
#ifndef MAKEINTRESOURCEW
#define MAKEINTRESOURCEW(res_to_make) ((LPWSTR)((ULONG_PTR)((WORD)(res_to_make))))
#endif
#ifndef LOBYTE
#define LOBYTE(w) ((BYTE) (((DWORD_PTR) (w)) & 0xff))
#endif
#ifndef HIBYTE
#define HIBYTE(w) ((BYTE) ((((DWORD_PTR) (w)) >> 8) & 0xff))
#endif
#ifndef LOWORD
#define LOWORD(l) ((WORD) (((DWORD_PTR) (l)) & 0xffff))
#endif
#ifndef HIWORD
#define HIWORD(l) ((WORD) ((((DWORD_PTR) (l)) >> 16) & 0xffff))
#endif
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif
#ifndef GET_XBUTTON_WPARAM
#define GET_XBUTTON_WPARAM(w) (HIWORD(w))
#endif
#ifndef GET_WHEEL_DELTA_WPARAM
#define GET_WHEEL_DELTA_WPARAM ((short)HIWORD(wParam))
#endif
#ifndef GET_KEYSTATE_WPARAM
#define GET_KEYSTATE_WPARAM (LOWORD(wParam))
#endif
#ifndef IS_HIGH_SURROGATE
#define IS_HIGH_SURROGATE(x) (((x) >= 0xd800) && ((x) <= 0xdbff))
#endif
#ifndef IS_LOW_SURROGATE
#define IS_LOW_SURROGATE(x) (((x) >= 0xdc00) && ((x) <= 0xdfff))
#endif
#ifndef IS_SURROGATE_PAIR
#define IS_SURROGATE_PAIR(h, l) (IS_HIGH_SURROGATE(h) && IS_LOW_SURROGATE(l))
#endif
#ifndef RGB
#define RGB ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
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
#ifndef UNICODE_NOCHAR
#define UNICODE_NOCHAR 0xFFFF
#endif
#ifndef USER_TIMER_MINIMUM
#define USER_TIMER_MINIMUM 0x0000000A
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
#ifndef NO_ERROR
#define NO_ERROR 0
#endif
#ifndef ERROR_GEN_FAILURE
#define ERROR_GEN_FAILURE 0x1F
#endif
#ifndef INVALID_FILE_SIZE
#define INVALID_FILE_SIZE ((DWORD)0xffffffff)
#endif
#ifndef IMAGE_CURSOR
#define IMAGE_CURSOR 2
#endif
#ifndef HTCLIENT
#define HTCLIENT 1
#endif
#ifndef OCR_NORMAL
#define OCR_NORMAL 32512
#endif
#ifndef OCR_IBEAM
#define OCR_IBEAM 32513
#endif
#ifndef OCR_WAIT
#define OCR_WAIT 32514
#endif
#ifndef OCR_CROSS
#define OCR_CROSS 32515
#endif
#ifndef OCR_UP
#define OCR_UP 32516
#endif
#ifndef OCR_SIZENWSE
#define OCR_SIZENWSE 32642
#endif
#ifndef OCR_SIZENESW
#define OCR_SIZENESW 32643
#endif
#ifndef OCR_SIZEWE
#define OCR_SIZEWE 32644
#endif
#ifndef OCR_SIZENS
#define OCR_SIZENS 32645
#endif
#ifndef OCR_SIZEALL
#define OCR_SIZEALL 32646
#endif
#ifndef OCR_NO
#define OCR_NO 32648
#endif
#ifndef OCR_HAND
#define OCR_HAND 32649
#endif
#ifndef OCR_APPSTARTING
#define OCR_APPSTARTING 32650
#endif
#ifndef TRANSPARENT
#define TRANSPARENT 1
#endif
#ifndef OPAQUE
#define OPAQUE 2
#endif
#ifndef NULL_BRUSH
#define NULL_BRUSH 5
#endif
#ifndef PS_SOLID
#define PS_SOLID 0
#endif 
#ifndef HALFTONE
#define HALFTONE 4
#endif
#ifndef STRETCH_HALFTONE
#define STRETCH_HALFTONE HALFTONE
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
#ifndef SRCCOPY
#define SRCCOPY (DWORD)0x00CC0020
#endif
#ifndef GENERIC_WRITE
#define GENERIC_WRITE 0x40000000
#endif
#ifndef GENERIC_READ
#define GENERIC_READ 0x80000000
#endif
#ifndef FILE_SHARE_READ
#define FILE_SHARE_READ 0x00000001
#endif
#ifndef FILE_SHARE_WRITE
#define FILE_SHARE_WRITE 0x00000002
#endif
#ifndef CREATE_NEW
#define CREATE_NEW 1
#endif
#ifndef CREATE_NEW
#define CREATE_NEW 1
#endif
#ifndef CREATE_ALWAYS
#define CREATE_ALWAYS 2
#endif
#ifndef OPEN_EXISTING
#define OPEN_EXISTING 3
#endif
#ifndef OPEN_ALWAYS
#define OPEN_ALWAYS 4
#endif
#ifndef TRUNCATE_EXISTING
#define TRUNCATE_EXISTING 5
#endif
#ifndef FILE_BEGIN
#define FILE_BEGIN 0
#endif
#ifndef FILE_CURRENT
#define FILE_CURRENT 1
#endif
#ifndef FILE_END
#define FILE_END 2
#endif
#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE ((HANDLE) (LONG_PTR)-1)
#endif
#ifndef FILE_SHARE_DELETE
#define FILE_SHARE_DELETE 0x00000004
#endif
#ifndef FILE_ATTRIBUTE_NORMAL
#define FILE_ATTRIBUTE_NORMAL 0x00000080
#endif
#ifndef FILE_FLAG_POSIX_SEMANTICS
#define FILE_FLAG_POSIX_SEMANTICS 0x1000000
#endif
#ifndef FILE_FLAG_OVERLAPPED
#define FILE_FLAG_OVERLAPPED 0x40000000
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
#ifndef SWP_NOSENDCHANGING
#define SWP_NOSENDCHANGING 0x0400
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
#ifndef SIZE_MAXIMIZED
#define SIZE_MAXIMIZED 2
#endif
#ifndef SIZE_MINIMIZED
#define SIZE_MINIMIZED 1
#endif
#ifndef USER_TIMER_MINIMUM
#define USER_TIMER_MINIMUM 0x0000000A
#endif
#ifndef SC_MINIMIZE
#define SC_MINIMIZE 0xF020
#endif
#ifndef SC_MAXIMIZE
#define SC_MAXIMIZE 0xF030
#endif
#ifndef SC_CLOSE
#define SC_CLOSE 0xF060
#endif
#ifndef SC_VSCROLL
#define SC_VSCROLL 0xF070
#endif
#ifndef SC_HSCROLL
#define SC_HSCROLL 0xF080
#endif
#ifndef SC_KEYMENU
#define SC_KEYMENU 0xF100
#endif
#ifndef SC_RESTORE
#define SC_RESTORE 0xF120
#endif
#ifndef MK_LBUTTON
#define MK_LBUTTON 0x0001
#endif
#ifndef MK_RBUTTON
#define MK_RBUTTON 0x0002
#endif
#ifndef MK_MBUTTON
#define MK_MBUTTON 0x0010
#endif
#ifndef MK_XBUTTON1
#define MK_XBUTTON1 0x0020
#endif
#ifndef MK_XBUTTON2
#define MK_XBUTTON2 0x0040
#endif
#ifndef XBUTTON1
#define XBUTTON1 0x0001
#endif
#ifndef XBUTTON2
#define XBUTTON1 0x0002
#endif
#ifndef VK_ENTER
#define VK_ENTER 10
#endif
#ifndef VK_OEM_NEC_EQUAL
#define VK_OEM_NEC_EQUAL 0x92
#endif
#ifndef WHEEL_DELTA
#define WHEEL_DELTA 120
#endif
#ifndef MAPVK_VK_TO_VSC_EX
#define MAPVK_VK_TO_VSC_EX 4
#endif
#ifndef HWND_BOTTOM
#define HWND_BOTTOM (HWND)1
#endif
#ifndef HWND_NOTOPMOST
#define HWND_NOTOPMOST (HWND)(-2)
#endif
#ifndef HWND_TOP
#define HWND_TOP (HWND)0
#endif
#ifndef HWND_TOPMOST
#define HWND_BOTTOM (HWND)(-1)
#endif
#ifndef WM_NCCREATE
#define WM_NCCREATE 0x0081
#endif
#ifndef WM_NCCALCSIZE
#define WM_NCCALCSIZE 0x0083
#endif
#ifndef WM_NCACTIVATE
#define WM_NCACTIVATE 0x0086
#endif
#ifndef WM_NCLBUTTONDOWN
#define WM_NCLBUTTONDOWN 0x00A1
#endif
#ifndef WM_CAPTURECHANGED
#define WM_CAPTURECHANGED 0x0215
#endif
#ifndef WM_GETMINMAXINFO
#define WM_GETMINMAXINFO 0x0024
#endif
#ifndef WM_ENTERSIZEMOVE
#define WM_ENTERSIZEMOVE 0x0231
#endif
#ifndef WM_ENTERMENULOOP
#define WM_ENTERMENULOOP 0x0211
#endif
#ifndef WM_GETDPISCALEDSIZE
#define WM_GETDPISCALEDSIZE 0x02E4
#endif
#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0
#endif
#ifndef WM_SETTINGCHANGE
#define WM_SETTINGCHANGE 0x001A
#endif
#ifndef WM_TIMER
#define WM_TIMER 0x0113
#endif
#ifndef WM_TOUCH
#define WM_TOUCH 0x0240
#endif
#ifndef WM_EXITSIZEMOVE
#define WM_EXITSIZEMOVE 0x0232
#endif
#ifndef WM_DROPFILES
#define WM_DROPFILES 0x0233
#endif
#ifndef WM_DISPLAYCHANGE
#define WM_DISPLAYCHANGE 0x007E
#endif
#ifndef WM_EXITMENULOOP
#define WM_EXITMENULOOP 0x0212
#endif
#ifndef WM_SETCURSOR
#define WM_SETCURSOR 0x0020
#endif
#ifndef WM_ACTIVATE
#define WM_ACTIVATE 0x0006
#endif
#ifndef WM_ENTERIDLE
#define WM_ENTERIDLE 0x0121
#endif
#ifndef WM_UNICHAR
#define WM_UNICHAR 0x0109
#endif
#ifndef WM_CHAR
#define WM_CHAR 0x0102
#endif
#ifndef WM_INPUTLANGCHANGE
#define WM_INPUTLANGCHANGE 0x0051
#endif
#ifndef WM_THEMECHANGED
#define WM_THEMECHANGED 0x031A
#endif
#ifndef WM_DESTROY
#define WM_DESTROY 0x0002
#endif
#ifndef WM_CLOSE
#define WM_CLOSE 0x0010
#endif
#ifndef WM_QUIT
#define WM_QUIT 0x0012
#endif
#ifndef WM_PAINT
#define WM_PAINT 0x000F
#endif
#ifndef WM_WINDOWPOSCHANGING
#define WM_WINDOWPOSCHANGING 0x0046
#endif
#ifndef WM_WINDOWPOSCHANGED
#define WM_WINDOWPOSCHANGED 0x0047
#endif
#ifndef WM_SIZING
#define WM_SIZING 0x0214
#endif
#ifndef WM_SIZE
#define WM_SIZE 0x0005
#endif
#ifndef WM_SYSCOMMAND
#define WM_SYSCOMMAND 0x012
#endif
#ifndef WM_MOUSEMOVE
#define WM_MOUSEMOVE 0x0200
#endif
#ifndef WM_LBUTTONDOWN
#define WM_LBUTTONDOWN 0x0201
#endif
#ifndef WM_LBUTTONUP
#define WM_LBUTTONUP 0x0202
#endif
#ifndef WM_RBUTTONDOWN
#define WM_RBUTTONDOWN 0x0204
#endif
#ifndef WM_RBUTTONUP
#define WM_RBUTTONUP 0x0205
#endif
#ifndef WM_MBUTTONDOWN
#define WM_MBUTTONDOWN 0x0207
#endif
#ifndef WM_MBUTTONUP
#define WM_MBUTTONUP 0x0208
#endif
#ifndef WM_XBUTTONDOWN
#define WM_XBUTTONDOWN 0x020B
#endif
#ifndef WM_XBUTTONUP
#define WM_XBUTTONUP 0x020C
#endif
#ifndef WM_ERASEBKGND
#define WM_ERASEBKGND 0x0014
#endif
#ifndef WM_SETFOCUS
#define WM_SETFOCUS 0x0007
#endif
#ifndef WM_KILLFOCUS
#define WM_KILLFOCUS 0x0008
#endif
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef WM_KEYDOWN
#define WM_KEYDOWN 0x0100
#endif
#ifndef WM_KEYUP
#define WM_KEYUP 0x0101
#endif
#ifndef WM_SYSKEYDOWN
#define WM_SYSKEYDOWN 0x0104
#endif
#ifndef WM_SYSKEYUP
#define WM_SYSKEYUP 0x0105
#endif
#ifndef WM_INPUT
#define WM_INPUT 0x00FF
#endif
#ifndef WM_POINTERUPDATE
#define WM_POINTERUPDATE 0x0245
#endif
#ifndef WM_SETCURSOR
#define WM_SETCURSOR 0x0020
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

typedef enum {
    OMG_WIN_APPMODE_DEFAULT,
    OMG_WIN_APPMODE_ALLOW_DARK,
    OMG_WIN_APPMODE_FORCE_DARK,
    OMG_WIN_APPMODE_FORCE_LIGHT,
    OMG_WIN_APPMODE_MAX
} OMG_WinPreferredAppMode;

typedef enum {
    OMG_WIN_PROCESS_DPI_UNAWARE = 0,
    OMG_WIN_PROCESS_SYSTEM_DPI_AWARE = 1,
    OMG_WIN_PROCESS_PER_MONITOR_DPI_AWARE = 2
} OMG_WIN_PROCESS_DPI_AWARENESS;

typedef struct {
    LONGLONG QuadPart;
} OMG_WIN_LARGE_INTEGER;

typedef struct {
    POINT ptReserved;
    POINT ptMaxSize;
    POINT ptMaxPosition;
    POINT ptMinTrackSize;
    POINT ptMaxTrackSize;
} OMG_WIN_MINMAXINFO, *OMG_WIN_PMINMAXINFO, *OMG_WIN_LPMINMAXINFO;

#if !OMG_WINAPI_DYNAMIC && !OMG_WINAPI_DYNAMIC_UGLY && !OMG_HAS_STD
void OMG_WIN_STD_PREFIX RtlGetVersion(OMG_WIN_NTDLL_OSVERSIONINFOEXW* version_information);
double OMG_WIN_STD_PREFIX floor(double);
double OMG_WIN_STD_PREFIX ceil(double);
double OMG_WIN_STD_PREFIX sin(double);
double OMG_WIN_STD_PREFIX cos(double);
double OMG_WIN_STD_PREFIX tan(double);
double OMG_WIN_STD_PREFIX atan(double);
double OMG_WIN_STD_PREFIX atan2(double, double);
double OMG_WIN_STD_PREFIX sqrt(double);
void OMG_WIN_STD_PREFIX qsort(void*, size_t, size_t, int (*) (const void*, const void*));
int OMG_WIN_STD_PREFIX abs(int n);
double OMG_WIN_STD_PREFIX fabs(double n);
double OMG_WIN_STD_PREFIX pow(double, double);
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
    BOOL OMG_WIN_STD_PREFIX (*QueryPerformanceFrequency)(OMG_WIN_LARGE_INTEGER*);
    BOOL OMG_WIN_STD_PREFIX (*QueryPerformanceCounter)(OMG_WIN_LARGE_INTEGER*);
    DWORD OMG_WIN_STD_PREFIX (*GetTickCount)(void);
    ULONGLONG OMG_WIN_STD_PREFIX (*GetTickCount64)(void);
    BOOL OMG_WIN_STD_PREFIX (*CloseHandle)(HANDLE);
    HANDLE OMG_WIN_STD_PREFIX (*CreateFileW)(LPCWSTR, DWORD, DWORD, void*, DWORD, DWORD, HANDLE);
    DWORD OMG_WIN_STD_PREFIX (*GetFileSize)(HANDLE, LPDWORD);
    DWORD OMG_WIN_STD_PREFIX (*GetFileSizeEx)(HANDLE, PLARGE_INTEGER);
    BOOL OMG_WIN_STD_PREFIX (*SetFilePointerEx)(HANDLE, LARGE_INTEGER, PLARGE_INTEGER, DWORD);
    BOOL OMG_WIN_STD_PREFIX (*CancelSynchronousIo)(HANDLE);
    BOOL OMG_WIN_STD_PREFIX (*ReadFile)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
    BOOL OMG_WIN_STD_PREFIX (*WriteFile)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
    uint64_t _tick64_emu;
} OMG_Kernel32;

typedef struct {
    HANDLE handle;
    void OMG_WIN_STD_PREFIX (*RtlGetVersion)(OMG_WIN_NTDLL_OSVERSIONINFOEXW*);
    double OMG_WIN_STD_PREFIX (*floor)(double);
    double OMG_WIN_STD_PREFIX (*ceil)(double);
    double OMG_WIN_STD_PREFIX (*sin)(double);
    double OMG_WIN_STD_PREFIX (*cos)(double);
    double OMG_WIN_STD_PREFIX (*tan)(double);
    double OMG_WIN_STD_PREFIX (*atan)(double);
    double OMG_WIN_STD_PREFIX (*atan2)(double, double);
    double OMG_WIN_STD_PREFIX (*sqrt)(double);
    void OMG_WIN_STD_PREFIX (*qsort)(void*, size_t, size_t, int (*) (const void*, const void*));
    int OMG_WIN_STD_PREFIX (*abs)(int);
    double OMG_WIN_STD_PREFIX (*fabs)(double);
    double OMG_WIN_STD_PREFIX (*pow)(double, double);
} OMG_Ntdll;

#ifdef SetWindowLongW
// TODO: fuck microsoft
#undef SetWindowLongW
#endif

typedef struct {
    HANDLE handle;
    UINT OMG_WIN_STD_PREFIX (*MapVirtualKeyW)(UINT, UINT);
    ATOM OMG_WIN_STD_PREFIX (*RegisterClassExW)(const WNDCLASSEXW*);
    BOOL OMG_WIN_STD_PREFIX (*UnregisterClassW)(LPCWSTR, HINSTANCE hInstance);
    HWND OMG_WIN_STD_PREFIX (*CreateWindowExW)(DWORD, LPCWSTR, LPCWSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
    BOOL OMG_WIN_STD_PREFIX (*DestroyWindow)(HWND);
    LRESULT OMG_WIN_STD_PREFIX (*SendMessageW)(HWND, UINT, WPARAM, LPARAM);
    void OMG_WIN_STD_PREFIX (*PostQuitMessage)(int);
    UINT_PTR OMG_WIN_STD_PREFIX (*SetTimer)(HWND, UINT_PTR, UINT_PTR, void*);
    BOOL OMG_WIN_STD_PREFIX (*KillTimer)(HWND, UINT_PTR);
    LONG OMG_WIN_STD_PREFIX (*SetWindowLongW)(HWND, int, LONG);
    LONG OMG_WIN_STD_PREFIX (*GetWindowLongW)(HWND, int);
#ifndef SetWindowLongPtrW
    LONG_PTR OMG_WIN_STD_PREFIX (*SetWindowLongPtrW)(HWND, int, LONG_PTR);
    LONG_PTR OMG_WIN_STD_PREFIX (*GetWindowLongPtrW)(HWND, int);
#endif
    HWND OMG_WIN_STD_PREFIX (*GetDesktopWindow)(void);
    HWND OMG_WIN_STD_PREFIX (*GetForegroundWindow)(void);
    BOOL OMG_WIN_STD_PREFIX (*GetWindowRect)(HWND, LPRECT);
    BOOL OMG_WIN_STD_PREFIX (*GetClientRect)(HWND, LPRECT);
    BOOL OMG_WIN_STD_PREFIX (*SetWindowPos)(HWND, HWND, int, int, int, int, UINT);
    BOOL OMG_WIN_STD_PREFIX (*MoveWindow)(HWND, int, int, int, int, BOOL);
    BOOL OMG_WIN_STD_PREFIX (*ShowWindow)(HWND, INT);
    BOOL OMG_WIN_STD_PREFIX (*SetWindowTextW)(HWND, LPCWSTR);
    BOOL OMG_WIN_STD_PREFIX (*WaitMessage)(void);
    BOOL OMG_WIN_STD_PREFIX (*PeekMessageW)(LPMSG, HWND, UINT, UINT, UINT);
    BOOL OMG_WIN_STD_PREFIX (*TranslateMessage)(const MSG*);
    LRESULT OMG_WIN_STD_PREFIX (*DispatchMessageW)(const MSG*);
    BOOL OMG_WIN_STD_PREFIX (*InvalidateRect)(HWND, const RECT*, BOOL);
    BOOL OMG_WIN_STD_PREFIX (*InvalidateRgn)(HWND, HRGN, BOOL);
    BOOL OMG_WIN_STD_PREFIX (*UpdateWindow)(HWND);
    LRESULT OMG_WIN_STD_PREFIX (*DefWindowProcW)(HWND, UINT, WPARAM, LPARAM);
    HBRUSH OMG_WIN_STD_PREFIX (*GetSysColorBrush)(int);
    HANDLE OMG_WIN_STD_PREFIX (*LoadImageW)(HINSTANCE, LPCWSTR, UINT, int, int, UINT);
    BOOL OMG_WIN_STD_PREFIX (*SetProcessDPIAware)(void);
    HRESULT OMG_WIN_STD_PREFIX (*SetProcessDpiAwareness)(OMG_WIN_PROCESS_DPI_AWARENESS);
    BOOL OMG_WIN_STD_PREFIX (*SetProcessDpiAwarenessContext)(DPI_AWARENESS_CONTEXT);
    BOOL OMG_WIN_STD_PREFIX (*EnableNonClientDpiScaling)(HWND);
    UINT OMG_WIN_STD_PREFIX (*GetDpiForSystem)(void);
    UINT OMG_WIN_STD_PREFIX (*GetDpiForWindow)(HWND);
    int OMG_WIN_STD_PREFIX (*GetSystemMetrics)(int);
    int OMG_WIN_STD_PREFIX (*GetSystemMetricsForDpi)(int, UINT);
    HDC OMG_WIN_STD_PREFIX (*GetDC)(HWND);
    HDC OMG_WIN_STD_PREFIX (*GetWindowDC)(HWND);
    int OMG_WIN_STD_PREFIX (*ReleaseDC)(HWND, HDC);
    HDC OMG_WIN_STD_PREFIX (*BeginPaint)(HWND, LPPAINTSTRUCT);
    HDC OMG_WIN_STD_PREFIX (*EndPaint)(HWND, const PAINTSTRUCT*);
    int OMG_WIN_STD_PREFIX (*FillRect)(HDC, const RECT*, HBRUSH);
    int OMG_WIN_STD_PREFIX (*FrameRect)(HDC, const RECT*, HBRUSH);
    BOOL OMG_WIN_STD_PREFIX (*SetCursorPos)(int, int);
    HWND OMG_WIN_STD_PREFIX (*SetFocus)(HWND);
    BOOL OMG_WIN_STD_PREFIX (*SetForegroundWindow)(HWND);
    BOOL OMG_WIN_STD_PREFIX (*SetSystemCursor)(HCURSOR, DWORD);
    BOOL OMG_WIN_STD_PREFIX (*SetCursor)(HCURSOR);
    int OMG_WIN_STD_PREFIX (*ShowCursor)(BOOL);
    BOOL OMG_WIN_STD_PREFIX (*DestroyCursor)(HCURSOR);
    BOOL OMG_WIN_STD_PREFIX (*ClipCursor)(const RECT*);
} OMG_User32;

typedef struct {
    HANDLE handle;
    int OMG_WIN_STD_PREFIX (*GetDeviceCaps)(HDC, int);
    HDC OMG_WIN_STD_PREFIX (*CreateCompatibleDC)(HDC);
    BOOL OMG_WIN_STD_PREFIX (*DeleteDC)(HDC);
    HBITMAP OMG_WIN_STD_PREFIX (*CreateCompatibleBitmap)(HDC, int, int);
    BOOL OMG_WIN_STD_PREFIX (*DeleteObject)(HGDIOBJ);
    HBRUSH OMG_WIN_STD_PREFIX (*CreateSolidBrush)(COLORREF);
    HGDIOBJ OMG_WIN_STD_PREFIX (*SelectObject)(HDC, HGDIOBJ);
    int OMG_WIN_STD_PREFIX (*SetBkMode)(HDC, int);
    COLORREF OMG_WIN_STD_PREFIX (*SetBkColor)(HDC, COLORREF);
    COLORREF OMG_WIN_STD_PREFIX (*SetPixel)(HDC, int, int, COLORREF);
    BOOL OMG_WIN_STD_PREFIX (*Rectangle)(HDC, int, int, int, int);
    BOOL OMG_WIN_STD_PREFIX (*Ellipse)(HDC, int, int, int, int);
    BOOL OMG_WIN_STD_PREFIX (*BitBlt)(HDC, int, int, int, int, HDC, int, int, DWORD);
    BOOL OMG_WIN_STD_PREFIX (*StretchBlt)(HDC, int, int, int, int, HDC, int, int, int, int, DWORD);
    int OMG_WIN_STD_PREFIX (*SetStretchBltMode)(HDC, int);
    HGDIOBJ OMG_WIN_STD_PREFIX (*GetStockObject)(int);
    HPEN OMG_WIN_STD_PREFIX (*CreatePen)(int, int, COLORREF);
} OMG_Gdi32;

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
OMG_API bool omg_winapi_gdi32_load(OMG_Gdi32* this);
OMG_API bool omg_winapi_gdi32_free(OMG_Gdi32* this);
OMG_API bool omg_winapi_ntdll_load(OMG_Ntdll* this);
OMG_API bool omg_winapi_ntdll_free(OMG_Ntdll* this);
OMG_API bool omg_winapi_dwmapi_load(OMG_Dwmapi* this);
OMG_API bool omg_winapi_dwmapi_free(OMG_Dwmapi* this);
OMG_API bool omg_winapi_uxtheme_load(OMG_Uxtheme* this, int build_num);
OMG_API bool omg_winapi_uxtheme_free(OMG_Uxtheme* this);
#if OMG_EXPORT_SHIT
OMG_API ULONGLONG omg_win_get_tick_count64_emu(void);
#endif
#endif
