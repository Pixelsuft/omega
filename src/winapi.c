#include <omega/winapi.h>

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

#if OMG_IS_WIN
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
    LOAD_REQUIRED(WriteConsoleW);
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
    return false;
#endif
}

bool omg_winapi_ntdll_load(OMG_Ntdll* this) {
#if OMG_WINAPI_DYNAMIC
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
#if OMG_WINAPI_DYNAMIC
    if (this->handle == NULL)
        return true;
    return (bool)FreeLibrary(this->handle);
#else
    return false;
#endif
}
#endif
