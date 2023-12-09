#include <omega/staticapi.h>
#if OMG_IS_WIN
#include <omega/winapi.h>
#endif
#if OMG_SUPPORTS_SDL2
#include <omega/sdl2.h>
#endif

void* omg_static_lib_load(const OMG_String* fn, const wchar_t* adv_fn) {
#if OMG_SUPPORTS_SDL2 && !OMG_SDL2_DYNAMIC
    OMG_UNUSED(adv_fn);
    omg_string_ensure_null((OMG_String*)fn);
    return SDL_LoadObject(fn->ptr);
#elif OMG_IS_WIN
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
#else
    OMG_UNUSED(fn, adv_fn);
    return NULL;
#endif
}

void* omg_static_lib_func(void* lib, const OMG_String* func_name) {
#if OMG_SUPPORTS_SDL2 && !OMG_SDL2_DYNAMIC
    omg_string_ensure_null((OMG_String*)func_name);
    return SDL_LoadFunction(lib, func_name->ptr);
#elif OMG_IS_WIN
    omg_string_ensure_null((OMG_String*)func_name);
    OMG_BEGIN_POINTER_CAST();
    return (void*)((size_t)GetProcAddress((HMODULE)lib, func_name->ptr));
    OMG_END_POINTER_CAST();
#else
    OMG_UNUSED(lib, func_name);
    return NULL;
#endif
}

bool omg_static_lib_free(void* lib) {
#if OMG_SUPPORTS_SDL2 && !OMG_SDL2_DYNAMIC
    SDL_UnloadObject(lib);
    return false;
#elif OMG_IS_WIN
    return (bool)FreeLibrary((HMODULE)lib);
#else
    OMG_UNUSED(lib);
    return true;
#endif
}