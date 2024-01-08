#include <omega/image.h>

bool omg_image_loader_destroy(OMG_ImageLoader* this) {
    this->inited = false;
    return false;
}

bool omg_image_loader_init(OMG_ImageLoader* this) {
    this->inited = true;
    this->type = OMG_IMAGE_LOADER_TYPE_NONE;
    this->destroy = omg_image_loader_destroy;
    return false;
}
