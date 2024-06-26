#include <omega/api_static.h>
#if OMG_SUPPORT_SDL2 && !OMG_SDL2_DYNAMIC
#include <omega/api_sdl2.h>
#elif OMG_IS_WIN
#include <omega/api_win.h>
#elif OMG_IS_UNIX && OMG_HAS_STD
#if defined(__linux__)
#include <gnu/lib-names.h>
#endif
#include <dlfcn.h>
#include <stdlib.h>
#endif

void* omg_static_lib_load(const OMG_String* fn, const wchar_t* adv_fn) {
#if OMG_SUPPORT_SDL2 && !OMG_SDL2_DYNAMIC
    OMG_UNUSED(adv_fn);
    char* fn_ptr;
    if (OMG_ISNULL(fn)) {
#ifdef LIBC_SO
        fn_ptr = LIBC_SO;
#else
        fn_ptr = "libc.so";
#endif
    }
    else {
        omg_string_ensure_null((OMG_String*)fn);
        fn_ptr = fn->ptr;
    }
    return SDL_LoadObject(fn_ptr);
#elif OMG_IS_WIN
    void* result = NULL;
    if (OMG_ISNOTNULL(adv_fn)) {
        result = (void*)LoadLibraryExW(adv_fn, NULL, LOAD_IGNORE_CODE_AUTHZ_LEVEL);
    }
#if OMG_WIN_BETTER_LIBRARY_LOAD
    if (OMG_ISNULL(fn)) {
        result = (void*)LoadLibraryExW(L"ntdll.dll", NULL, LOAD_IGNORE_CODE_AUTHZ_LEVEL | LOAD_LIBRARY_SEARCH_SYSTEM32);
    }
    else {
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
    }
#endif
    if (OMG_ISNULL(result)) {
        omg_string_ensure_null((OMG_String*)fn);
        result = (void*)LoadLibraryExA(fn->ptr, NULL, LOAD_IGNORE_CODE_AUTHZ_LEVEL);
    }
    return result;
#elif OMG_IS_UNIX && OMG_HAS_STD
    OMG_UNUSED(adv_fn);
    char* fn_ptr;
    if (OMG_ISNULL(fn)) {
#ifdef LIBC_SO
        fn_ptr = LIBC_SO;
#else
        fn_ptr = "libc.so";
#endif
    }
    else {
        omg_string_ensure_null((OMG_String*)fn);
        fn_ptr = fn->ptr;
    }
    void* res = dlopen(fn_ptr, RTLD_GLOBAL | RTLD_NOW);
    // printf("%s %s %p\n", fn->ptr, dlerror(), res);
    return res;
#else
    OMG_UNUSED(fn, adv_fn);
    return NULL;
#endif
}

void* omg_static_lib_func(void* lib, const OMG_String* func_name) {
#if OMG_SUPPORT_SDL2 && !OMG_SDL2_DYNAMIC
    omg_string_ensure_null((OMG_String*)func_name);
    return SDL_LoadFunction(lib, func_name->ptr);
#elif OMG_IS_WIN
    omg_string_ensure_null((OMG_String*)func_name);
    OMG_BEGIN_POINTER_CAST();
    return (void*)((size_t)GetProcAddress((HMODULE)lib, func_name->ptr));
    OMG_END_POINTER_CAST();
#elif OMG_IS_UNIX && OMG_HAS_STD
    omg_string_ensure_null((OMG_String*)func_name);
    return dlsym(lib, func_name->ptr);
#else
    OMG_UNUSED(lib, func_name);
    return NULL;
#endif
}

bool omg_static_lib_free(void* lib) {
#if OMG_SUPPORT_SDL2 && !OMG_SDL2_DYNAMIC
    SDL_UnloadObject(lib);
    return false;
#elif OMG_IS_WIN
    return (bool)FreeLibrary((HMODULE)lib);
#elif OMG_IS_UNIX && OMG_HAS_STD
    return (bool)dlclose(lib);
#else
    OMG_UNUSED(lib);
    return true;
#endif
}

#if OMG_SUPPORT_STATIC_MEM
void* omg_static_malloc(size_t size) {
#if OMG_SUPPORT_SDL2 && !OMG_SDL2_DYNAMIC
    return SDL_malloc(size);
#elif OMG_IS_WIN
    return HeapAlloc(GetProcessHeap(), 0, size);
#elif OMG_HAS_STD
    return malloc(size);
#else
    OMG_UNUSED(size);
    return NULL;
#endif
}

void omg_static_free(void* ptr) {
#if OMG_SUPPORT_SDL2 && !OMG_SDL2_DYNAMIC
    SDL_free(ptr);
#elif OMG_IS_WIN
    HeapFree(GetProcessHeap(), 0, ptr);
#elif OMG_HAS_STD
    free(ptr);
#else
    OMG_UNUSED(ptr);
#endif
}
#endif
