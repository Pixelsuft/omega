#pragma once
#include <omega/api_sdl2_image.h>

#if OMG_SUPPORT_SDL2_IMAGE
#include <omega/image.h>

typedef struct {
    OMG_ImageLoader parent;
    OMG_Sdl2Image* img;
} OMG_ImageLoaderSdl2;

OMG_API bool omg_image_loader_sdl2_init(OMG_ImageLoaderSdl2* this);
#endif
