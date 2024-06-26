#pragma once
#include <omega/api_sdl2_image.h>

#if OMG_SUPPORT_SDL2_IMAGE
#include <omega/api_sdl2.h>
#include <omega/image.h>

typedef struct {
    OMG_ImageLoader parent;
    OMG_Sdl2Image img;
    OMG_Sdl2* sdl2;
} OMG_ImageLoaderSdl2;

OMG_API bool omg_image_loader_sdl2_init(OMG_ImageLoaderSdl2* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_image_loader_sdl2_destroy(OMG_ImageLoaderSdl2* this);
OMG_API bool omg_image_loader_sdl2_image_from(OMG_ImageLoaderSdl2* this, int type, const void* data, void* buf, int format);
#endif
#endif
