#include <omega/config.h>

#if OMG_SUPPORT_OMG_IMAGE
#include <omega/image.h>
#include <omega/api_spng.h>

typedef struct {
    OMG_ImageLoader parent;
#if OMG_SUPPORT_SPNG
    OMG_Spng spng;
#endif
} OMG_ImageLoaderOmg;

OMG_API bool omg_image_loader_omg_init(OMG_ImageLoaderOmg* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_image_loader_omg_destroy(OMG_ImageLoaderOmg* this);
OMG_API bool omg_image_loader_omg_image_from(OMG_ImageLoaderOmg* this, int type, const void* data, void* buf, int format);
#endif
#endif
