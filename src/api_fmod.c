#include <omega/api_fmod.h>
#if OMG_SUPPORT_FMOD
#if OMG_FMOD_DYNAMIC
#include <omega/api_static.h>
#define LOAD_REQUIRED(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#endif

bool omg_fmod_dll_load(OMG_Fmod* this, const OMG_String* dll_path) {
#if OMG_SDL2_MIXER_DYNAMIC
    if (OMG_ISNULL(dll_path))
#if OMG_IS_WIN
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("fmod.dll"), L"fmod.dll");
#else
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("libfmod.so"), NULL);
#endif
    else
        this->handle = omg_static_lib_load(dll_path, NULL);
    if (OMG_ISNULL(this->handle))
        return true;
#else
    OMG_UNUSED(dll_path);
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED(FMOD_System_Create);
    LOAD_REQUIRED(FMOD_System_Release);
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_fmod_dll_free(OMG_Fmod* this) {
#if OMG_SDL2_MIXER_DYNAMIC
    return omg_static_lib_free(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}
#endif
