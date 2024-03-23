#include <omega/image_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/omega_raylib.h>
#define base ((OMG_ImageLoader*)this)
#define omg_base ((OMG_Omega*)base->omg)

#define _FORMAT_TO_EXT(format, ext) do { \
    if ((format & OMG_IMG_FORMAT_NONE) || (format & OMG_IMG_FORMAT_AUTO) || (format & OMG_IMG_FORMAT_PNG)) \
        ext = ".png"; \
    else if (format & OMG_IMG_FORMAT_JPG) \
        ext = ".jpg"; \
    else if (format & OMG_IMG_FORMAT_TIF) \
        ext = ".tif"; \
    else if (format & OMG_IMG_FORMAT_WEBP) \
        ext = ".webp"; \
    else if (format & OMG_IMG_FORMAT_JXL) \
        ext = ".jxl"; \
    else if (format & OMG_IMG_FORMAT_AVIF) \
        ext = ".avif"; \
    else \
        ext = ".bmp"; \
} while (0)

bool omg_image_loader_raylib_image_from(OMG_ImageLoaderRaylib* this, int type, const void* data, void* buf, int format) {
    OMG_UNUSED(format); // TODO
    Image img;
    if (type == 0) {
        OMG_String* str_data = (OMG_String*)data;
        if (omg_string_ensure_null(str_data))
            return true;
        img = this->raylib->LoadImage(str_data->ptr);
    }
    else if (type == 1) {
        OMG_DataWithSize* mem_data = (OMG_DataWithSize*)data;
        char* ext;
        _FORMAT_TO_EXT(format, ext);
        img = this->raylib->LoadImageFromMemory(ext, (unsigned char*)mem_data->data, (int)mem_data->size);
    }
    else
        return true;
    if (!this->raylib->IsImageReady(img)) {
        _OMG_LOG_ERROR(omg_base, "Failed to load raylib image");
        return true;
    }
    omg_base->std->memcpy(buf, &img, sizeof(Image));
    return false;
}

bool omg_image_loader_raylib_init(OMG_ImageLoaderRaylib* this) {
    omg_image_loader_init(base);
    base->type = OMG_IMAGE_LOADER_TYPE_RAYLIB;
    OMG_BEGIN_POINTER_CAST();
    base->image_from_internal = omg_image_loader_raylib_image_from;
    OMG_END_POINTER_CAST();
    this->raylib = ((OMG_OmegaRaylib*)base->omg)->raylib;
    return false;
}
#endif
