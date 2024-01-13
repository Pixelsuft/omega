#pragma once
#include <omega/ostd.h>

#define OMG_REN_TEXTURE_FROM_FILE(ren, path) ren->tex_from_surf(ren, ((OMG_Omega*)ren->omg)->winmgr->surf_from_path(((OMG_Omega*)ren->omg)->winmgr, path, OMG_IMG_FORMAT_AUTO), true)

#define _OMG_NULL_TEXTURE_WARN() _OMG_LOG_WARN(omg_base, "Attempted to free null texture")

#define OMG_TEXTURE_ACCESS_STATIC 1
#define OMG_TEXTURE_ACCESS_TARGET 2
#define OMG_TEXTURE_ACCESS_STREAMING 3

typedef struct {
    void* extra1;
    OMG_FPoint size;
    bool has_alpha;
} OMG_Texture;
