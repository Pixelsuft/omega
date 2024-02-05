#pragma once
#include <omega/ostd.h>

#define _OMG_NULL_SURFACE_WARN() _OMG_LOG_WARN(omg_base, "Attempted to free null surface")
#define OMG_IS_DUMMY_SURF(surf) (OMG_ISNULL((surf)) || ((surf)->size.w <= 0.0f))
#define _OMG_NULL_SURF_CHECK(surf) do { \
    if (OMG_IS_DUMMY_SURF(surf)) \
        return true; \
} while (0)

typedef struct {
    void* data;
    OMG_FPoint size;
    bool has_alpha;
#if OMG_ALLOW_SURF_WAS_ALLOCATED
    bool was_allocated;
#endif
} OMG_Surface;
