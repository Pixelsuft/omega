#include <omega/config.h>

#if OMG_SUPPORT_OMG_IMAGE
#include <omega/image.h>

typedef struct {
    OMG_ImageLoader parent;
} OMG_ImageLoaderOmg;

OMG_API bool omg_image_loader_omg_init(OMG_ImageLoaderOmg* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_image_loader_omg_image_from_fp(OMG_ImageLoaderOmg* this, const OMG_String* path, void* buf, int format);
#endif
#endif
