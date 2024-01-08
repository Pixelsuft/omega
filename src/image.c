#include <omega/image.h>

bool omg_image_loader_destroy(OMG_ImageLoader* this) {
    this->inited = false;
    return false;
}

void* omg_image_loader_image_from_fp(OMG_ImageLoader* this, const OMG_String* path) {
    OMG_UNUSED(this, path);
    return NULL;
}

bool omg_image_loader_init(OMG_ImageLoader* this) {
    this->inited = true;
    this->type = OMG_IMAGE_LOADER_TYPE_NONE;
    this->destroy = omg_image_loader_destroy;
    this->image_from_fp_internal = omg_image_loader_image_from_fp;
    return false;
}
