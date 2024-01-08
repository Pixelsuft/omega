#include <omega/image_sdl2.h>

#if OMG_SUPPORT_SDL2_IMAGE
#define base ((OMG_ImageLoader*)this)

bool omg_image_loader_sdl2_init(OMG_ImageLoaderSdl2* this) {
    omg_image_loader_init(base);
    return false;
}
#endif
