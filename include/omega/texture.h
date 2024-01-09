#pragma once
#include <omega/ostd.h>

#if OMG_NULL_TEX_CHECK
#define _OMG_NULL_TEX_CHECK() do { \
    if (OMG_ISNULL(tex)) \
        return true; \
} while (0)
#else
#define _OMG_NULL_TEX_CHECK()
#endif

#define _OMG_NULL_TEXTURE_WARN() _OMG_LOG_WARN(omg_base, "Attempted to free null texture")

#define OMG_TEXTURE_ACCESS_STATIC 1
#define OMG_TEXTURE_ACCESS_TARGET 2
#define OMG_TEXTURE_ACCESS_STREAMING 3

typedef struct {
    void* extra1;
    OMG_FPoint size;
    bool has_alpha;
} OMG_Texture;
