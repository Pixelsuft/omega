#pragma once
#include <omega/ostd.h>

#define _OMG_NULL_TEXTURE_WARN() _OMG_LOG_WARN(omg_base, "Attempted to free null texture")

#define OMG_TEXTURE_ACCESS_STATIC 1
#define OMG_TEXTURE_ACCESS_TARGET 2
#define OMG_TEXTURE_ACCESS_STREAMING 3

typedef struct {
    OMG_FPoint size;
    bool auto_blend;
    bool has_alpha;
} OMG_Texture;
