#include <omega/winapi.h>

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

void* omg_win_std_lib_load(const OMG_String* fn, const wchar_t* adv_fn) {
    void* result = NULL;
    if (OMG_ISNOTNULL(adv_fn)) {
        return (void*)LoadLibraryExW(adv_fn, NULL, LOAD_IGNORE_CODE_AUTHZ_LEVEL);
    }
    if (OMG_ISNULL(fn))
        return NULL;
#if OMG_WIN_BETTER_LIBRARY_LOAD
    wchar_t* out_buf = HeapAlloc(GetProcessHeap(), 0, fn->len * 4 + 2);
    if (OMG_ISNOTNULL(out_buf)) {
        int res = MultiByteToWideChar(WIN_CP_UTF8, 0, fn->ptr, fn->len, out_buf, (int)(fn->len * 4));
        if (res > 0) {
            // TODO: fn->len or res?
            out_buf[res] = L'\0';
            result = (void*)LoadLibraryExW(out_buf, NULL, LOAD_IGNORE_CODE_AUTHZ_LEVEL);
        }
        HeapFree(GetProcessHeap(), 0, out_buf);
    }
#endif
    if (OMG_ISNULL(result)) {
        omg_string_ensure_null((OMG_String*)fn);
        result = (void*)LoadLibraryExA(fn->ptr, NULL, LOAD_IGNORE_CODE_AUTHZ_LEVEL);
    }
    return result;
}

void* omg_win_std_lib_func(void* lib, const OMG_String* func_name) {
    omg_string_ensure_null((OMG_String*)func_name);
    OMG_BEGIN_POINTER_CAST();
    return (void*)((size_t)GetProcAddress((HMODULE)lib, func_name->ptr));
    OMG_END_POINTER_CAST();
}

bool omg_win_std_lib_free(void* lib) {
    return (bool)FreeLibrary((HMODULE)lib);
}
#endif
