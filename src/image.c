#include <omega/image.h>

bool omg_image_loader_destroy(OMG_ImageLoader* this) {
    this->inited = false;
    return false;
}

bool omg_image_loader_image_from_fp(OMG_ImageLoader* this, const OMG_String* path, void* buf, int format) {
    OMG_UNUSED(this, path, buf, format);
    return true;
}

bool omg_image_loader_init(OMG_ImageLoader* this) {
    this->inited = false;
    this->type = OMG_IMAGE_LOADER_TYPE_NONE;
    this->prefer_alpha = true;
    this->destroy = omg_image_loader_destroy;
    this->image_from_fp_internal = omg_image_loader_image_from_fp;
    return false;
}
