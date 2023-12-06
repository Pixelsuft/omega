#include <omega/sdl2.h>
#if OMG_SUPPORTS_SDL2
#include <omega/ostd.h>
#if OMG_SDL2_DYNAMIC
#if OMG_IS_WIN
#include <omega/winapi.h>
#define LOAD_REQUIRED(func_name) this->func_name = (omg_win_std_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#endif
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#endif

bool omg_sdl2_dll_load(OMG_Sdl2* this, const OMG_String* dll_path) {
#if OMG_SDL2_DYNAMIC
#if OMG_IS_WIN
    if (OMG_ISNULL(dll_path))
        this->handle = omg_win_std_lib_load(&OMG_STRING_MAKE_STATIC("SDL2.dll"), L"SDL2.dll");
    else
        this->handle = omg_win_std_lib_load(dll_path, NULL);
#endif
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
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_sdl2_dll_free(OMG_Sdl2* this) {
#if OMG_SDL2_DYNAMIC
#if OMG_IS_WIN
    return omg_win_std_lib_free(this->handle);
#endif
    return true;
#else
    OMG_UNUSED(this);
    return false;
#endif
}

#endif