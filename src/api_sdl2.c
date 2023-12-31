#include <omega/api_sdl2.h>
#if OMG_SUPPORT_SDL2
#include <omega/ostd.h>
#if OMG_SDL2_DYNAMIC
#include <omega/api_static.h>
#define LOAD_REQUIRED(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#if OMG_SDL2_COMOMG_SDL2_COMPAT_STATIC
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = NULL
#else
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = func_name
#endif
#endif

static OMG_Sdl2* omg_sdl2_cache = NULL;

uint64_t omg_sdl2_get_ticks64_emu(void) {
    return (uint64_t)omg_sdl2_cache->SDL_GetTicks();
}

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
    LOAD_REQUIRED(SDL_strcmp);
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
    LOAD_REQUIRED(SDL_floor);
    LOAD_REQUIRED(SDL_ceil);
    LOAD_REQUIRED(SDL_round);
    LOAD_REQUIRED(SDL_floorf);
    LOAD_REQUIRED(SDL_ceilf);
    LOAD_REQUIRED(SDL_roundf);
    LOAD_REQUIRED(SDL_malloc);
    LOAD_REQUIRED(SDL_realloc);
    LOAD_REQUIRED(SDL_free);
    LOAD_REQUIRED(SDL_LogInfo);
    LOAD_REQUIRED(SDL_LogWarn);
    LOAD_REQUIRED(SDL_LogError);
    LOAD_REQUIRED(SDL_LogCritical);
    LOAD_REQUIRED(SDL_GetError);
    LOAD_REQUIRED(SDL_GetMouseState);
    LOAD_REQUIRED(SDL_GetRelativeMouseState);
    LOAD_REQUIRED(SDL_CreateWindow);
    LOAD_REQUIRED(SDL_CreateWindowFrom);
    LOAD_REQUIRED(SDL_DestroyWindow);
    LOAD_REQUIRED(SDL_GetWindowSize);
    LOAD_REQUIRED(SDL_GetWindowID);
    LOAD_REQUIRED(SDL_ShowWindow);
    LOAD_REQUIRED(SDL_HideWindow);
    LOAD_REQUIRED(SDL_SetWindowFullscreen);
    LOAD_REQUIRED(SDL_SetWindowBordered);
    LOAD_REQUIRED(SDL_MinimizeWindow);
    LOAD_REQUIRED(SDL_MaximizeWindow);
    LOAD_REQUIRED(SDL_RestoreWindow);
    LOAD_REQUIRED(SDL_SetWindowTitle);
    LOAD_REQUIRED(SDL_PollEvent);
    LOAD_REQUIRED(SDL_GetTicks);
    LOAD_REQUIRED(SDL_GetPerformanceFrequency);
    LOAD_REQUIRED(SDL_GetPerformanceCounter);
    LOAD_REQUIRED(SDL_GetNumRenderDrivers);
    LOAD_REQUIRED(SDL_GetRenderDriverInfo);
    LOAD_REQUIRED(SDL_GetRendererInfo);
    LOAD_REQUIRED(SDL_CreateRenderer);
    LOAD_REQUIRED(SDL_GetRendererOutputSize);
    LOAD_REQUIRED(SDL_DestroyRenderer);
    LOAD_REQUIRED(SDL_RenderClear);
    LOAD_REQUIRED(SDL_SetRenderDrawColor);
    LOAD_REQUIRED(SDL_RenderPresent);
    // TODO: check using SDL2 versions
    LOAD_REQUIRED_COMPAT(SDL_SetWindowResizable); // 2.0.5
    LOAD_REQUIRED_COMPAT(SDL_GetNumAllocations); // 2.0.7
    LOAD_REQUIRED_COMPAT(SDL_SetWindowAlwaysOnTop); // 2.0.16
    LOAD_REQUIRED_COMPAT(SDL_GetTicks64); // 2.0.18
    if (OMG_ISNULL(this->SDL_GetTicks64))
        this->SDL_GetTicks64 = omg_sdl2_get_ticks64_emu;
    OMG_END_POINTER_CAST();
    omg_sdl2_cache = this;
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
