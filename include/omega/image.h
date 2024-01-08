#pragma once
#include <omega/ostd.h>

#define OMG_IMAGE_LOADER_TYPE_NONE 0
#define OMG_IMAGE_LOADER_TYPE_SDL2 1

typedef struct OMG_ImageLoader {
    int type;
    bool (*init)(struct OMG_ImageLoader* this);
    void* (*image_from_fp_internal)(struct OMG_ImageLoader* this, const OMG_String* path);
    bool (*destroy)(struct OMG_ImageLoader* this);
    bool inited;
    bool was_allocated;
} OMG_ImageLoader;

OMG_API bool omg_image_loader_init(OMG_ImageLoader* this);
OMG_API bool omg_image_loader_destroy(OMG_ImageLoader* this);
#if OMG_EXPORT_SHIT
OMG_API void* omg_image_loader_image_from_fp(OMG_ImageLoader* this, const OMG_String* path);
#endif
