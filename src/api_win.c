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
#if OMG_WINAPI_DYNAMIC_UGLY
#define LOAD_REQUIRED_UGLY(func_name) this->func_name = GetProcAddress(this->handle, #func_name)
#else
#define LOAD_REQUIRED_UGLY(func_name) this->func_name = func_name
#endif

bool omg_winapi_kernel32_load(OMG_Kernel32* this) {
#if OMG_WINAPI_DYNAMIC
    this->handle = LOAD_SYSTEM_LIBRARY(L"kernel32.dll");
    if (this->handle == NULL)
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
    if (this->handle == NULL)
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
    if (this->handle == NULL)
        return true;
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED_UGLY(RtlGetVersion);
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_winapi_ntdll_free(OMG_Ntdll* this) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_UGLY
    if (this->handle == NULL)
        return true;
    return (bool)FreeLibrary(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_winapi_user32_load(OMG_User32* this) {
#if OMG_WINAPI_DYNAMIC
    this->handle = LOAD_SYSTEM_LIBRARY(L"user32.dll");
    if (this->handle == NULL)
        return true;
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED(SetProcessDPIAware);
    LOAD_REQUIRED(GetDpiForSystem);
    LOAD_REQUIRED(GetDpiForWindow);
    LOAD_REQUIRED(GetSystemMetricsForDpi);
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_winapi_user32_free(OMG_User32* this) {
#if OMG_WINAPI_DYNAMIC
    if (this->handle == NULL)
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
    if (this->handle == NULL)
        return true;
#endif
    OMG_BEGIN_POINTER_CAST();
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_winapi_dwmapi_free(OMG_Dwmapi* this) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT
    if (this->handle == NULL)
        return true;
    return (bool)FreeLibrary(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_winapi_uxtheme_load(OMG_Uxtheme* this) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT
    this->handle = LOAD_SYSTEM_LIBRARY(L"uxtheme.dll");
    if (this->handle == NULL)
        return true;
#endif
    OMG_BEGIN_POINTER_CAST();
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_winapi_uxtheme_free(OMG_Uxtheme* this) {
#if OMG_WINAPI_DYNAMIC || OMG_WINAPI_DYNAMIC_COMPAT
    if (this->handle == NULL)
        return true;
    return (bool)FreeLibrary(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}
#endif
