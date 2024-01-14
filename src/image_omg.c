#include <omega/image_omg.h>

#if OMG_SUPPORT_OMG_IMAGE
#if OMG_SUPPORT_LODEPNG
#include <omega_libs/lodepng.h>
#endif
#define base ((OMG_ImageLoader*)this)
#define omg_base ((OMG_Omega*)base->omg)

bool omg_image_loader_omg_image_from_fp(OMG_ImageLoaderOmg* this, const OMG_String* path, void* buf, int format) {
    OMG_UNUSED(this, path, buf, format);
    return true;
}

bool omg_image_loader_omg_init(OMG_ImageLoaderOmg* this) {
    omg_image_loader_init(base);
    base->type = OMG_IMAGE_LOADER_TYPE_OMG;
    OMG_BEGIN_POINTER_CAST();
    base->image_from_fp_internal = omg_image_loader_omg_image_from_fp;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
