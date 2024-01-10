#pragma once
#include <omega/api_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/image.h>

typedef struct {
    OMG_ImageLoader parent;
    OMG_Raylib* raylib;
} OMG_ImageLoaderRaylib;

OMG_API bool omg_image_loader_raylib_init(OMG_ImageLoaderRaylib* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_image_loader_raylib_image_from_fp(OMG_ImageLoaderRaylib* this, const OMG_String* path, void* buf);
#endif
#endif