#include <omega/api_win.h>

#if OMG_IS_WIN

#if OMG_WIN_BETTER_LIBRARY_LOAD
#include <stringapiset.h>
#include <heapapi.h>
#endif

#define LOAD_SYSTEM_LIBRARY(lib_path) LoadLibraryExW(lib_path, NULL, LOAD_IGNORE_CODE_AUTHZ_LEVEL | LOAD_LIBRARY_SEARCH_SYSTEM32)
#if OMG_WINAPI_DYNAMIC
// TODO: fail check
#define LOAD_REQUIRED(func_name) this->func_name = GetProcAddress(this->handle, #func_name)
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#endif
#if OMG_WINAPI_DYNAMIC_COMPAT
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = GetProcAddress(this->handle, #func_name)
#elif OMG_WINAPI_STATIC_COMPAT
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = func_name
#else
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = NULL
#endif
#if OMG_WINAPI_DYNAMIC_UGLY
#define LOAD_REQUIRED_UGLY(func_name) this->func_name = GetProcAddress(this->handle, #func_name)
#else
#define LOAD_REQUIRED_UGLY(func_name) this->func_name = func_name
#endif
#if OMG_WINAPI_DYNAMIC_ORDINAL
#define LOAD_REQUIRED_ORD(func_name, func_ord) this->func_name = GetProcAddress(this->handle, OMG_WIN_MAKEINTRESOURCEA(func_ord))
#else
#define LOAD_REQUIRED_ORD(func_name, func_ord) this->func_name = NULL
#endif

bool omg_winapi_kernel32_load(OMG_Kernel32* this) {
#if OMG_WINAPI_DYNAMIC
    this->handle = LOAD_SYSTEM_LIBRARY(L"kernel32.dll");
    if (OMG_ISNULL(this->handle))
        return true;
    OMG_BEGIN_POINTER_CAST();
#endif
    LOAD_REQUIRED(LoadLibraryExW);
    LOAD_REQUIRED(GetProcAddress);
    LOAD_REQUIRED(FreeLibrary);
    LOAD_REQUIRED(HeapCreate);
    LOAD_REQUIRED(HeapDestroy);
    LOAD_REQUIRED(HeapAlloc);
    LOAD_REQUIRED(HeapReAlloc);
    LOAD_REQUIRED(HeapFree);
    LOAD_REQUIRED(GetLastError);
    LOAD_REQUIRED(GetStdHandle);
    LOAD_REQUIRED(AllocConsole);
    LOAD_REQUIRED(FreeConsole);
    LOAD_REQUIRED(AttachConsole);
    LOAD_REQUIRED(MultiByteToWideChar);
    LOAD_REQUIRED(WideCharToMultiByte);
    LOAD_REQUIRED(WriteConsoleW);
    LOAD_REQUIRED(FormatMessageW);
#if OMG_WINAPI_DYNAMIC
    OMG_END_POINTER_CAST();
#endif
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
    LOAD_REQUIRED_COMPAT(SetProcessDPIAware);
    LOAD_REQUIRED_COMPAT(GetDpiForSystem);
    LOAD_REQUIRED_COMPAT(GetDpiForWindow);
    LOAD_REQUIRED_COMPAT(GetSystemMetricsForDpi);
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

bool omg_winapi_dwmapi_load(OMG_Dwmapi* this) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT
    this->handle = LOAD_SYSTEM_LIBRARY(L"dwmapi.dll");
    if (OMG_ISNULL(this->handle))
        return true;
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED_COMPAT(DwmSetWindowAttribute);
    LOAD_REQUIRED_COMPAT(DwmFlush);
    OMG_END_POINTER_CAST();
    return false;
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
    if (OMG_ISNULL(this->handle))
        return true;
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
        this->ShouldAppsUseDarkMode = NULL;
        this->IsDarkModeAllowedForApp = NULL;
    }
    OMG_END_POINTER_CAST();
    return false;
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
#endif
