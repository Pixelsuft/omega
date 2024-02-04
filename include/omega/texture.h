#pragma once
#include <omega/ostd.h>

#define OMG_REN_TEXTURE_FROM_FILE(ren, path) ren->tex_from_surf(ren, NULL, ((OMG_Omega*)ren->omg)->winmgr->surf_from_fp(((OMG_Omega*)ren->omg)->winmgr, NULL, path, OMG_IMG_FORMAT_AUTO), true)
#define OMG_IS_DUMMY_TEX(tex) (OMG_ISNULL((tex)) || ((tex)->size.w <= 0.0f))
#define OMG_IS_DUMMY_TEX_NN(tex) (OMG_ISNOTNULL((tex)) && ((tex)->size.w <= 0.0f))
#define _OMG_NULL_TEXTURE_WARN() _OMG_LOG_WARN(omg_base, "Attempted to free null texture")
#define _OMG_NULL_TEXTURE_CHECK(tex) do { \
    if (OMG_IS_DUMMY_TEX(tex)) \
        return true; \
} while (0)

#define OMG_TEXTURE_ACCESS_STATIC 1
#define OMG_TEXTURE_ACCESS_TARGET 2
#define OMG_TEXTURE_ACCESS_STREAMING 3

typedef struct {
    void* extra1;
    OMG_FPoint size;
    bool has_alpha;
#if OMG_ALLOW_TEX_WAS_ALLOCATED
    bool was_allocated;
#endif
} OMG_Texture;
