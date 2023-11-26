#include <omega/winapi.h>

#define LOAD_SYSTEM_LIBRARY_F(lib_path) LoadLibraryExW(lib_path, NULL, LOAD_IGNORE_CODE_AUTHZ_LEVEL | LOAD_LIBRARY_SEARCH_SYSTEM32)
#define LOAD_SYSTEM_LIBRARY(lib_path) k32->LoadLibraryExW(lib_path, NULL, LOAD_IGNORE_CODE_AUTHZ_LEVEL | LOAD_LIBRARY_SEARCH_SYSTEM32)
#if OMG_WINAPI_DYNAMIC
// TODO: fail check
#define LOAD_REQUIRED(func_name) this->func_name = GetProcAddress(this->handle, #func_name)
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#endif

#if OMG_IS_WIN
bool omg_winapi_kernel32_load(OMG_Kernel32* this) {
#if OMG_WINAPI_DYNAMIC
    this->handle = LOAD_SYSTEM_LIBRARY_F(L"kernel32.dll");
    if (this->handle == NULL)
        return true;
    OMG_BEGIN_POINTER_CAST();
#endif
    LOAD_REQUIRED(LoadLibraryExW);
    LOAD_REQUIRED(FreeLibrary);
    LOAD_REQUIRED(HeapCreate);
    LOAD_REQUIRED(HeapDestroy);
    LOAD_REQUIRED(HeapAlloc);
    LOAD_REQUIRED(HeapFree);
#if OMG_WINAPI_DYNAMIC
    OMG_END_POINTER_CAST();
#endif
    return false;
}

bool omg_winapi_kernel32_free(OMG_Kernel32* this) {
#if OMG_WINAPI_DYNAMIC
    if (this->handle == NULL || this->FreeLibrary == NULL)
        return true;
    return (bool)this->FreeLibrary(this->handle);
#else
    return false;
#endif
}

bool omg_winapi_ntdll_load(OMG_Ntdll* this, OMG_Kernel32* k32) {
#if OMG_WINAPI_DYNAMIC
    this->handle = LOAD_SYSTEM_LIBRARY(L"ntdll.dll");
    if (this->handle == NULL)
        return true;
    OMG_BEGIN_POINTER_CAST();
#endif
    LOAD_REQUIRED(RtlGetVersion);
#if OMG_WINAPI_DYNAMIC
    OMG_END_POINTER_CAST();
#endif
    return false;
}

bool omg_winapi_ntdll_free(OMG_Ntdll* this, OMG_Kernel32* k32) {
#if OMG_WINAPI_DYNAMIC
    if (this->handle == NULL || k32->FreeLibrary == NULL)
        return true;
    return (bool)k32->FreeLibrary(this->handle);
#else
    return false;
#endif
}
#endif
