#pragma once
#include <omega/ostd.h>

#define OMG_IMAGE_LOADER_TYPE_NONE 0
#define OMG_IMAGE_LOADER_TYPE_SDL2 1

#define OMG_IMG_FORMAT_BMP (1 << 1)
#define OMG_IMG_FORMAT_JPG (1 << 2)
#define OMG_IMG_FORMAT_PNG (1 << 3)
#define OMG_IMG_FORMAT_TIF (1 << 4)
#define OMG_IMG_FORMAT_WEBP (1 << 5)
#define OMG_IMG_FORMAT_JXL (1 << 6)
#define OMG_IMG_FORMAT_AVIF (1 << 7)

typedef struct OMG_ImageLoader {
    int type;
    void* omg;
    bool (*init)(struct OMG_ImageLoader* this);
    bool (*image_from_fp_internal)(struct OMG_ImageLoader* this, const OMG_String* path, void* buf);
    bool (*destroy)(struct OMG_ImageLoader* this);
    bool inited;
    bool was_allocated;
} OMG_ImageLoader;

OMG_API bool omg_image_loader_init(OMG_ImageLoader* this);
OMG_API bool omg_image_loader_destroy(OMG_ImageLoader* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_image_loader_image_from_fp(OMG_ImageLoader* this, const OMG_String* path, void* buf);
#endif
