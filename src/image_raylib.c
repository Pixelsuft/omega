#include <omega/image_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/omega_raylib.h>
#define base ((OMG_ImageLoader*)this)
#define omg_base ((OMG_Omega*)base->omg)

bool omg_image_loader_raylib_image_from(OMG_ImageLoaderRaylib* this, const OMG_String* path, void* buf, int format) {
    OMG_UNUSED(format);
    if (omg_string_ensure_null((OMG_String*)path))
        return true;
    Image img = this->raylib->LoadImage(path->ptr);
    if (!this->raylib->IsImageReady(img)) {
        _OMG_LOG_ERROR(omg_base, "Failed to load raylib image ", path->ptr);
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
