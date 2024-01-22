#pragma once
#include <omega/api_sdl2_ttf.h>

#if OMG_SUPPORT_SDL2_TTF
#include <omega/font.h>

typedef struct {
    OMG_Font parent;
    TTF_Font* font;
} OMG_FontSdl2;

typedef struct {
    OMG_FontMgr parent;
    OMG_Sdl2Ttf ttf;
} OMG_FontMgrSdl2;

OMG_API bool omg_fontmgr_sdl2_init(OMG_FontMgrSdl2* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_fontmgr_font_sdl2_destroy(OMG_FontMgrSdl2* this, OMG_FontSdl2* font);
OMG_API OMG_FontSdl2* omg_fontmgr_sdl2_font_from_fp(OMG_FontMgrSdl2* this, OMG_FontSdl2* font, const OMG_String* fp, long index, float size);
OMG_API bool omg_fontmgr_sdl2_destroy(OMG_FontMgrSdl2* this);
OMG_API bool omg_fontmgr_sdl2_font_set_scale(OMG_FontMgrSdl2* this, OMG_FontSdl2* font, const OMG_FPoint* scale);
#endif
#endif
