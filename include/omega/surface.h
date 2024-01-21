#pragma once
#include <omega/ostd.h>
#define _OMG_NULL_SURFACE_WARN() _OMG_LOG_WARN(omg_base, "Attempted to free null surface")

typedef struct {
    void* data;
    OMG_FPoint size;
    bool has_alpha;
#if OMG_ALLOW_SURF_WAS_ALLOCATED
    bool was_allocated;
#endif
} OMG_Surface;
