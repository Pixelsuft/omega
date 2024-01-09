#include <omega/api_sdl2_mixer.h>
#if OMG_SUPPORT_SDL2_MIXER
#include <omega/ostd.h>
#if OMG_SDL2_MIXER_DYNAMIC
#include <omega/api_static.h>
#define LOAD_REQUIRED(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#if OMG_SDL2_MIXER_COMPAT_STATIC
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = NULL
#else
#define LOAD_REQUIRED_COMPAT(func_name) this->func_name = func_name
#endif
#endif

bool omg_sdl2_mixer_dll_load(OMG_Sdl2Mixer* this, const OMG_String* dll_path) {
#if OMG_SDL2_MIXER_DYNAMIC
    if (OMG_ISNULL(dll_path))
#if OMG_IS_WIN
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("SDL2_mixer.dll"), L"SDL2_mixer.dll");
#else
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("libSDL2_mixer.so"), NULL);
#endif
    else
        this->handle = omg_static_lib_load(dll_path, NULL);
    if (OMG_ISNULL(this->handle))
        return true;
#else
    OMG_UNUSED(dll_path);
#endif
    OMG_BEGIN_POINTER_CAST();
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_sdl2_mixer_dll_free(OMG_Sdl2Mixer* this) {
#if OMG_SDL2_MIXER_DYNAMIC
    return omg_static_lib_free(this->handle);
#else
    return false;
#endif
}
#endif
