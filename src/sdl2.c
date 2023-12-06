#include <omega/sdl2.h>
#if OMG_SUPPORTS_SDL2
#include <omega/ostd.h>
#if OMG_SDL2_DYNAMIC
#if OMG_IS_WIN
#include <omega/winapi.h>
#endif
#endif

bool omg_sdl2_dll_load(OMG_Sdl2* this, const OMG_String* dll_path) {
#if OMG_IS_WIN
    this->handle = omg_win_std_lib_load(OMG_ISNULL(dll_path) ? &OMG_STRING_MAKE_STATIC("SDL2.dll") : dll_path);
#endif
    return OMG_ISNULL(this->handle);
}

bool omg_sdl2_dll_free(OMG_Sdl2* this) {
#if OMG_IS_WIN
    return omg_win_std_lib_free(this->handle);
#endif
    return true;
}

#endif