#include <omega/image_sdl2.h>

#if OMG_SUPPORT_SDL2_IMAGE
#include <omega/omega.h>
#include <omega/omega_sdl2.h>
#define base ((OMG_ImageLoader*)this)
#define omg_base ((OMG_Omega*)base->omg)
#define IMG_GETERROR() ((omg_base->type == OMG_OMEGA_TYPE_SDL2) ? ((OMG_OmegaSdl2*)omg_base)->sdl2->SDL_GetError() : "")

bool omg_image_loader_sdl2_destroy(OMG_ImageLoaderSdl2* this) {
    if (!base->inited)
        return false;
    bool res = false;
    this->img.IMG_Quit();
    res = omg_sdl2_image_dll_free(&this->img) || res;
    omg_image_loader_destroy(base);
    return res;
}

void* omg_image_loader_sdl2_image_from_fp(OMG_ImageLoaderSdl2* this, const OMG_String* path) {
    OMG_UNUSED(this, path);
    return NULL;
}

bool omg_image_loader_sdl2_init(OMG_ImageLoaderSdl2* this) {
    omg_image_loader_init(base);
    if (omg_sdl2_image_dll_load(&this->img, omg_base->sdl2_image_dll_path)) {
        _OMG_LOG_ERROR(omg_base, "Failed to load SDL2_image dll");
        return true;
    }
    int sdl2_formats = 0;
    int need_format = omg_base->winmgr->image_formats & (~OMG_IMG_FORMAT_BMP);
    if (need_format & OMG_IMG_FORMAT_JPG)
        sdl2_formats |= IMG_INIT_JPG;
    if (need_format & OMG_IMG_FORMAT_PNG)
        sdl2_formats |= IMG_INIT_PNG;
    if (need_format & OMG_IMG_FORMAT_TIF)
        sdl2_formats |= IMG_INIT_TIF;
    if (need_format & OMG_IMG_FORMAT_WEBP)
        sdl2_formats |= IMG_INIT_WEBP;
    if (need_format & OMG_IMG_FORMAT_JXL)
        sdl2_formats |= IMG_INIT_JXL;
    if (need_format & OMG_IMG_FORMAT_AVIF)
        sdl2_formats |= IMG_INIT_AVIF;
    int res = this->img.IMG_Init(sdl2_formats);
    if ((need_format != 0) && (res == 0)) {
        _OMG_LOG_ERROR(omg_base, "Failed to init SDL2_image (", IMG_GETERROR(), ")");
        omg_sdl2_image_dll_free(&this->img);
        return true;
    }
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_image_loader_sdl2_destroy;
    base->image_from_fp_internal = omg_image_loader_sdl2_image_from_fp;
    OMG_END_POINTER_CAST();
    base->inited = true;
    return false;
}
#endif
