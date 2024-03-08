#include <omega/api_win.h>

#if OMG_IS_WIN

#if OMG_WIN_BETTER_LIBRARY_LOAD
#include <stringapiset.h>
#include <heapapi.h>
#endif

#ifdef _MSC_VER
// I hate Visual Studio
#define OMG_PROC_ADDRESS (void*)GetProcAddress
#else
#define OMG_PROC_ADDRESS GetProcAddress
#endif

#define LOAD_SYSTEM_LIBRARY(lib_path) LoadLibraryExW(lib_path, NULL, LOAD_IGNORE_CODE_AUTHZ_LEVEL | LOAD_LIBRARY_SEARCH_SYSTEM32)
#if OMG_WINAPI_DYNAMIC
// TODO: fail check
#define LOAD_REQUIRED(func_name) this->func_name = OMG_PROC_ADDRESS(this->handle, #func_name)
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#endif
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = (OMG_ISNULL(this->handle) ? NULL : OMG_PROC_ADDRESS(this->handle, #func_name))
#elif OMG_WINAPI_STATIC_COMPAT
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = NULL
#else
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = func_name
#endif
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_UGLY
#define LOAD_REQUIRED_UGLY(func_name) this->func_name = OMG_PROC_ADDRESS(this->handle, #func_name)
#else
#define LOAD_REQUIRED_UGLY(func_name) this->func_name = func_name
#endif
#if OMG_WINAPI_DYNAMIC_ORDINAL
#define LOAD_REQUIRED_ORD(func_name, func_ord) this->func_name = (OMG_ISNULL(this->handle) ? NULL : OMG_PROC_ADDRESS(this->handle, MAKEINTRESOURCEA(func_ord)))
#else
#define LOAD_REQUIRED_ORD(func_name, func_ord) this->func_name = NULL
#endif

#ifdef _MSC_VER
// TODO: avoid hack
#pragma warning( disable : 4047 )
#endif

static OMG_Kernel32* k32_cache = NULL;

ULONGLONG omg_win_get_tick_count64_emu(void) {
    uint64_t res = (uint64_t)k32_cache->GetTickCount();
    while (res < k32_cache->_tick64_emu) {
        res += (uint64_t)sizeof(uint32_t);
    }
    k32_cache->_tick64_emu = res;
    return res;
}

bool omg_winapi_kernel32_load(OMG_Kernel32* this) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT
    this->handle = LOAD_SYSTEM_LIBRARY(L"kernel32.dll");
    if (OMG_ISNULL(this->handle))
        return true;
    OMG_BEGIN_POINTER_CAST();
#endif
    LOAD_REQUIRED(LoadLibraryExW);
    LOAD_REQUIRED(ExitProcess);
    LOAD_REQUIRED(GetProcAddress);
    LOAD_REQUIRED(FreeLibrary);
    LOAD_REQUIRED(HeapCreate);
    LOAD_REQUIRED(HeapDestroy);
    LOAD_REQUIRED(HeapAlloc);
    LOAD_REQUIRED(HeapReAlloc);
    LOAD_REQUIRED(HeapFree);
    LOAD_REQUIRED(Sleep);
    LOAD_REQUIRED(GetLastError);
    LOAD_REQUIRED(MultiByteToWideChar);
    LOAD_REQUIRED(WideCharToMultiByte);
    LOAD_REQUIRED(WriteConsoleW);
    LOAD_REQUIRED(FormatMessageW);
    LOAD_REQUIRED(GetModuleHandleW);
    LOAD_REQUIRED(GetStdHandle);
    LOAD_REQUIRED(AllocConsole);
    LOAD_REQUIRED(FreeConsole);
    LOAD_REQUIRED(QueryPerformanceFrequency);
    LOAD_REQUIRED(QueryPerformanceCounter);
    LOAD_REQUIRED(GetTickCount);
    LOAD_REQUIRED(CloseHandle);
    LOAD_REQUIRED(CreateFileW);
    LOAD_REQUIRED(GetFileSize);
    LOAD_REQUIRED(GetEnvironmentVariableW);
    LOAD_REQUIRED(SetEnvironmentVariableW);
    LOAD_REQUIRED(CreateThread);
    LOAD_REQUIRED(GetCurrentThread);
    LOAD_REQUIRED(ExitThread);
    LOAD_REQUIRED(GetExitCodeThread);
    LOAD_REQUIRED(TerminateThread);
    LOAD_REQUIRED(GetThreadPriority);
    LOAD_REQUIRED(SetPriorityClass);
    LOAD_REQUIRED(SetThreadPriority);
    LOAD_REQUIRED(WaitForSingleObjectEx);
    LOAD_REQUIRED(GetCurrentThreadId);
    LOAD_REQUIRED(GetCurrentDirectoryW);
    LOAD_REQUIRED(GetModuleFileNameW);
    LOAD_REQUIRED_COMPAT(CreateFile2);
    LOAD_REQUIRED_COMPAT(CreateDirectoryW);
    LOAD_REQUIRED_COMPAT(AttachConsole);
    LOAD_REQUIRED_COMPAT(GetTickCount64);
    LOAD_REQUIRED_COMPAT(GetFileSizeEx);
    LOAD_REQUIRED_COMPAT(SetFilePointerEx);
    LOAD_REQUIRED_COMPAT(CancelSynchronousIo);
    LOAD_REQUIRED_COMPAT(GetFileAttributesW);
    LOAD_REQUIRED_COMPAT(ReadFile);
    LOAD_REQUIRED_COMPAT(WriteFile);
    LOAD_REQUIRED_COMPAT(DeleteFileW);
    LOAD_REQUIRED_COMPAT(RemoveDirectoryW);
    LOAD_REQUIRED_COMPAT(MoveFileW);
    if (OMG_ISNULL(this->GetTickCount64))
        this->GetTickCount64 = omg_win_get_tick_count64_emu;
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT
    OMG_END_POINTER_CAST();
#endif
    this->_tick64_emu = 0;
    k32_cache = this;
    return false;
}

bool omg_winapi_kernel32_free(OMG_Kernel32* this) {
#if OMG_WINAPI_DYNAMIC
    if (OMG_ISNULL(this->handle))
        return true;
    return (bool)FreeLibrary(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_winapi_ntdll_load(OMG_Ntdll* this) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_UGLY
    this->handle = LOAD_SYSTEM_LIBRARY(L"ntdll.dll");
    if (OMG_ISNULL(this->handle))
        return true;
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED_UGLY(RtlGetVersion);
    LOAD_REQUIRED_UGLY(floor);
    LOAD_REQUIRED_UGLY(ceil);
    LOAD_REQUIRED_UGLY(sin);
    LOAD_REQUIRED_UGLY(cos);
    LOAD_REQUIRED_UGLY(tan);
    LOAD_REQUIRED_UGLY(atan);
    LOAD_REQUIRED_UGLY(atan2);
    LOAD_REQUIRED_UGLY(sqrt);
    LOAD_REQUIRED_UGLY(qsort);
    LOAD_REQUIRED_UGLY(abs);
    LOAD_REQUIRED_UGLY(fabs);
    LOAD_REQUIRED_UGLY(pow);
    LOAD_REQUIRED_UGLY(sscanf_s);
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_winapi_ntdll_free(OMG_Ntdll* this) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_UGLY
    if (OMG_ISNULL(this->handle))
        return true;
    return (bool)FreeLibrary(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_winapi_user32_load(OMG_User32* this) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT
    this->handle = LOAD_SYSTEM_LIBRARY(L"user32.dll");
    if (OMG_ISNULL(this->handle))
        return true;
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED(MapVirtualKeyW);
    LOAD_REQUIRED(RegisterClassExW);
    LOAD_REQUIRED(UnregisterClassW);
    LOAD_REQUIRED(CreateWindowExW);
    LOAD_REQUIRED(DestroyWindow);
    LOAD_REQUIRED(SendMessageW);
    LOAD_REQUIRED(PostQuitMessage);
    LOAD_REQUIRED(SetTimer);
    LOAD_REQUIRED(KillTimer);
#ifdef SetWindowLongPtrW
    this->SetWindowLongPtrW = NULL;
    this->GetWindowLongPtrW = NULL;
#else
    LOAD_REQUIRED(SetWindowLongPtrW);
    LOAD_REQUIRED(GetWindowLongPtrW);
#endif
    LOAD_REQUIRED(SetWindowLongW);
    LOAD_REQUIRED(GetWindowLongW);
    LOAD_REQUIRED(GetForegroundWindow);
    LOAD_REQUIRED(GetDesktopWindow);
    LOAD_REQUIRED(GetWindowRect);
    LOAD_REQUIRED(GetClientRect);
    LOAD_REQUIRED(SetWindowPos);
    LOAD_REQUIRED(MoveWindow);
    LOAD_REQUIRED(ShowWindow);
    LOAD_REQUIRED(SetWindowTextW);
    LOAD_REQUIRED(WaitMessage);
    LOAD_REQUIRED(PeekMessageW);
    LOAD_REQUIRED(TranslateMessage);
    LOAD_REQUIRED(DispatchMessageW);
    LOAD_REQUIRED(InvalidateRect);
    LOAD_REQUIRED(InvalidateRgn);
    LOAD_REQUIRED(UpdateWindow);
    LOAD_REQUIRED(DefWindowProcW);
    LOAD_REQUIRED(GetSysColorBrush);
    LOAD_REQUIRED(LoadImageW);
    LOAD_REQUIRED(GetDC);
    LOAD_REQUIRED(GetWindowDC);
    LOAD_REQUIRED(ReleaseDC);
    LOAD_REQUIRED(BeginPaint);
    LOAD_REQUIRED(EndPaint);
    LOAD_REQUIRED(FillRect);
    LOAD_REQUIRED(FrameRect);
    LOAD_REQUIRED(SetCursorPos);
    LOAD_REQUIRED(SetFocus);
    LOAD_REQUIRED(SetForegroundWindow);
    LOAD_REQUIRED(SetSystemCursor);
    LOAD_REQUIRED(SetCursor);
    LOAD_REQUIRED(ShowCursor);
    LOAD_REQUIRED(DestroyCursor);
    LOAD_REQUIRED(ClipCursor);
    LOAD_REQUIRED(ScreenToClient);
    LOAD_REQUIRED(ClientToScreen);
    LOAD_REQUIRED(EnumDisplayMonitors);
    LOAD_REQUIRED(EnumDisplayDevicesW);
    LOAD_REQUIRED(UnionRect);
    LOAD_REQUIRED(MessageBoxW);
    LOAD_REQUIRED(EnumDisplaySettingsW);
    LOAD_REQUIRED(AnimateWindow);
    LOAD_REQUIRED_COMPAT(SetProcessDPIAware);
    LOAD_REQUIRED_COMPAT(SetProcessDpiAwareness);
    LOAD_REQUIRED_COMPAT(SetProcessDpiAwarenessContext);
    LOAD_REQUIRED_COMPAT(EnableNonClientDpiScaling);
    LOAD_REQUIRED_COMPAT(GetDpiForSystem);
    LOAD_REQUIRED_COMPAT(GetDpiForWindow);
    LOAD_REQUIRED_COMPAT(GetSystemMetrics);
    LOAD_REQUIRED_COMPAT(GetSystemMetricsForDpi);
    // LOAD_REQUIRED_COMPAT(QueryDisplayConfig);
    // LOAD_REQUIRED_COMPAT(DisplayConfigGetDeviceInfo);
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_winapi_user32_free(OMG_User32* this) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT
    if (OMG_ISNULL(this->handle))
        return true;
    return (bool)FreeLibrary(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}


bool omg_winapi_gdi32_load(OMG_Gdi32* this) {
#if OMG_WINAPI_DYNAMIC
    this->handle = LOAD_SYSTEM_LIBRARY(L"gdi32.dll");
    if (OMG_ISNULL(this->handle))
        return true;
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED(GetDeviceCaps);
    LOAD_REQUIRED(CreateCompatibleDC);
    LOAD_REQUIRED(DeleteDC);
    LOAD_REQUIRED(CreateCompatibleBitmap);
    LOAD_REQUIRED(DeleteObject);
    LOAD_REQUIRED(CreateSolidBrush);
    LOAD_REQUIRED(SelectObject);
    LOAD_REQUIRED(SetBkMode);
    LOAD_REQUIRED(SetBkColor);
    LOAD_REQUIRED(SetPixel);
    LOAD_REQUIRED(Rectangle);
    LOAD_REQUIRED(Ellipse);
    LOAD_REQUIRED(BitBlt);
    LOAD_REQUIRED(StretchBlt);
    LOAD_REQUIRED(SetStretchBltMode);
    LOAD_REQUIRED(GetStockObject);
    LOAD_REQUIRED(CreatePen);
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_winapi_gdi32_free(OMG_Gdi32* this) {
#if OMG_WINAPI_DYNAMIC
    if (OMG_ISNULL(this->handle))
        return true;
    return (bool)FreeLibrary(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_winapi_dwmapi_load(OMG_Dwmapi* this) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT
    this->handle = LOAD_SYSTEM_LIBRARY(L"dwmapi.dll");
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED_COMPAT(DwmSetWindowAttribute);
    LOAD_REQUIRED_COMPAT(DwmFlush);
    OMG_END_POINTER_CAST();
    return (OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT) && OMG_ISNULL(this->handle);
}

bool omg_winapi_dwmapi_free(OMG_Dwmapi* this) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT
    if (OMG_ISNULL(this->handle))
        return true;
    return (bool)FreeLibrary(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_winapi_uxtheme_load(OMG_Uxtheme* this, int build_num) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_ORDINAL
    this->handle = LOAD_SYSTEM_LIBRARY(L"uxtheme.dll");
#endif
    OMG_BEGIN_POINTER_CAST();
    if (build_num >= 17763) {
        LOAD_REQUIRED_ORD(ShouldAppsUseDarkMode, 132);
        LOAD_REQUIRED_ORD(AllowDarkModeForWindow, 133);
        if (build_num < 18362) {
            this->SetPreferredAppMode = NULL;
            LOAD_REQUIRED_ORD(AllowDarkModeForApp, 135);
        }
        else {
            this->AllowDarkModeForApp = NULL;
            LOAD_REQUIRED_ORD(SetPreferredAppMode, 135);
        }
        LOAD_REQUIRED_ORD(FlushMenuThemes, 136);
        LOAD_REQUIRED_ORD(RefreshImmersiveColorPolicyState, 104);
        LOAD_REQUIRED_ORD(IsDarkModeAllowedForWindow, 137);
        if (build_num >= 18290) {
            LOAD_REQUIRED_ORD(ShouldSystemUseDarkMode, 138);
        }
        else {
            this->ShouldSystemUseDarkMode = NULL;
        }
        if (build_num >= 18334) {
            LOAD_REQUIRED_ORD(IsDarkModeAllowedForApp, 139);
        }
        else {
            this->IsDarkModeAllowedForApp = NULL;
        }
    }
    else {
        this->ShouldAppsUseDarkMode = NULL;
        this->AllowDarkModeForWindow = NULL;
        this->AllowDarkModeForApp = NULL;
        this->SetPreferredAppMode = NULL;
        this->FlushMenuThemes = NULL;
        this->RefreshImmersiveColorPolicyState = NULL;
        this->IsDarkModeAllowedForWindow = NULL;
        this->ShouldSystemUseDarkMode = NULL;
        this->IsDarkModeAllowedForApp = NULL;
    }
    OMG_END_POINTER_CAST();
    return (OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT) && OMG_ISNULL(this->handle);
}

bool omg_winapi_uxtheme_free(OMG_Uxtheme* this) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_ORDINAL
    if (OMG_ISNULL(this->handle))
        return true;
    return (bool)FreeLibrary(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_winapi_msvcrt_load(OMG_Msvcrt* this) {
#if OMG_ALLOW_C_RUNTIME
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT
    this->handle = LOAD_SYSTEM_LIBRARY(L"msvcrt.dll");
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED_COMPAT(_beginthreadex);
    LOAD_REQUIRED_COMPAT(_endthreadex);
    OMG_END_POINTER_CAST();
    return (OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT) && OMG_ISNULL(this->handle);
#else
    this->_beginthreadex = NULL;
    this->_endthreadex = NULL;
    return false;
#endif
}

bool omg_winapi_msvcrt_free(OMG_Msvcrt* this) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT && OMG_ALLOW_C_RUNTIME
    if (OMG_ISNULL(this->handle))
        return false;
    return (bool)FreeLibrary(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}
#endif
