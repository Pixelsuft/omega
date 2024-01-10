#include <omega/api_sdl2_image.h>
#if OMG_SUPPORT_SDL2_IMAGE
#include <omega/ostd.h>
#if OMG_SDL2_IMAGE_DYNAMIC
#include <omega/api_static.h>
#define LOAD_REQUIRED(func_name) this->func_name = (omg_static_lib_func(this->handle, &OMG_STRING_MAKE_STATIC(#func_name)))
#else
#define LOAD_REQUIRED(func_name) this->func_name = func_name
#endif

bool omg_sdl2_image_dll_load(OMG_Sdl2Image* this, const OMG_String* dll_path) {
#if OMG_SDL2_IMAGE_DYNAMIC
    if (OMG_ISNULL(dll_path))
#if OMG_IS_WIN
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("SDL2_image.dll"), L"SDL2_image.dll");
#else
        this->handle = omg_static_lib_load(&OMG_STRING_MAKE_STATIC("libSDL2_image.so"), NULL);
#endif
    else
        this->handle = omg_static_lib_load(dll_path, NULL);
    if (OMG_ISNULL(this->handle))
        return true;
#else
    OMG_UNUSED(dll_path);
#endif
    OMG_BEGIN_POINTER_CAST();
    LOAD_REQUIRED(IMG_Init);
    LOAD_REQUIRED(IMG_Quit);
    LOAD_REQUIRED(IMG_LoadTyped_RW);
    LOAD_REQUIRED(IMG_Load);
    LOAD_REQUIRED(IMG_Load_RW);
    LOAD_REQUIRED(IMG_LoadTexture);
    LOAD_REQUIRED(IMG_LoadTexture_RW);
    LOAD_REQUIRED(IMG_LoadTextureTyped_RW);
    LOAD_REQUIRED(IMG_SavePNG);
    LOAD_REQUIRED(IMG_SavePNG_RW);
    LOAD_REQUIRED(IMG_SaveJPG);
    LOAD_REQUIRED(IMG_SaveJPG_RW);
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_sdl2_image_dll_free(OMG_Sdl2Image* this) {
#if OMG_SDL2_IMAGE_DYNAMIC
    return omg_static_lib_free(this->handle);
#else
    OMG_UNUSED(this);
    return false;
#endif
}
#endif
