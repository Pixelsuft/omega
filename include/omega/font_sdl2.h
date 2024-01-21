#pragma once
#include <omega/api_sdl2_ttf.h>

#if OMG_SUPPORT_SDL2_TTF
#include <omega/font.h>

typedef struct {
    OMG_Font parent;
} OMG_FontSdl2;

typedef struct {
    OMG_FontMgr parent;
    OMG_Sdl2Ttf ttf;
} OMG_FontMgrSdl2;

OMG_API bool omg_fontmgr_sdl2_init(OMG_FontMgrSdl2* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_fontmgr_sdl2_destroy(OMG_FontMgrSdl2* this);
#endif
#endif
