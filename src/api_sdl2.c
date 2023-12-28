#include <omega/api_sdl2.h>
#if OMG_SUPPORT_SDL2
#include <omega/ostd.h>
#if OMG_SDL2_DYNAMIC
#include <omega/api_static.h>
#define LOAD_REQUIRED(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#endif

bool omg_sdl2_dll_load(OMG_Sdl2* this, const OMG_String* dll_path) {
#if OMG_SDL2_DYNAMIC
    if (OMG_ISNULL(dll_path))
#if OMG_IS_WIN
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("SDL2.dll"), L"SDL2.dll");
#else
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("libSDL2.so"), NULL);
#endif
    else
        this->handle = omg_static_lib_load(dll_path, NULL);
    if (OMG_ISNULL(this->handle))
        return true;
#else
    OMG_UNUSED(dll_path);
    SDL_VERSION(&this->ver);
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED(SDL_GetVersion);
#if OMG_SDL2_DYNAMIC
    this->SDL_GetVersion(&this->ver);
#endif
    LOAD_REQUIRED(SDL_Init);
    LOAD_REQUIRED(SDL_Quit);
    LOAD_REQUIRED(SDL_Delay);
    LOAD_REQUIRED(SDL_memset);
    LOAD_REQUIRED(SDL_memcpy);
    LOAD_REQUIRED(SDL_memmove);
    LOAD_REQUIRED(SDL_memcmp);
    LOAD_REQUIRED(SDL_strlen);
    LOAD_REQUIRED(SDL_utf8strlen);
    LOAD_REQUIRED(SDL_utf8strnlen);
    LOAD_REQUIRED(SDL_strrev);
    LOAD_REQUIRED(SDL_itoa);
    LOAD_REQUIRED(SDL_ltoa);
    LOAD_REQUIRED(SDL_uitoa);
    LOAD_REQUIRED(SDL_ultoa);
    LOAD_REQUIRED(SDL_lltoa);
    LOAD_REQUIRED(SDL_ulltoa);
    LOAD_REQUIRED(SDL_wcslen);
    LOAD_REQUIRED(SDL_malloc);
    LOAD_REQUIRED(SDL_realloc);
    LOAD_REQUIRED(SDL_free);
    LOAD_REQUIRED(SDL_LogInfo);
    LOAD_REQUIRED(SDL_LogWarn);
    LOAD_REQUIRED(SDL_LogError);
    LOAD_REQUIRED(SDL_LogCritical);
    LOAD_REQUIRED(SDL_GetError);
    LOAD_REQUIRED(SDL_CreateWindow);
    LOAD_REQUIRED(SDL_CreateWindowFrom);
    LOAD_REQUIRED(SDL_DestroyWindow);
    LOAD_REQUIRED(SDL_GetWindowSize);
    LOAD_REQUIRED(SDL_GetWindowID);
    LOAD_REQUIRED(SDL_ShowWindow);
    LOAD_REQUIRED(SDL_HideWindow);
    LOAD_REQUIRED(SDL_SetWindowResizable);
    LOAD_REQUIRED(SDL_SetWindowBordered);
    LOAD_REQUIRED(SDL_SetWindowAlwaysOnTop);
    LOAD_REQUIRED(SDL_MinimizeWindow);
    LOAD_REQUIRED(SDL_MaximizeWindow);
    LOAD_REQUIRED(SDL_RestoreWindow);
    LOAD_REQUIRED(SDL_SetWindowTitle);
    LOAD_REQUIRED(SDL_PollEvent);
    LOAD_REQUIRED(SDL_GetNumRenderDrivers);
    LOAD_REQUIRED(SDL_GetRenderDriverInfo);
    LOAD_REQUIRED(SDL_GetRendererInfo);
    LOAD_REQUIRED(SDL_CreateRenderer);
    LOAD_REQUIRED(SDL_GetRendererOutputSize);
    LOAD_REQUIRED(SDL_DestroyRenderer);
    LOAD_REQUIRED(SDL_RenderClear);
    LOAD_REQUIRED(SDL_SetRenderDrawColor);
    LOAD_REQUIRED(SDL_RenderPresent);
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_sdl2_dll_free(OMG_Sdl2* this) {
#if OMG_SDL2_DYNAMIC
    return omg_static_lib_free(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}

#endif
