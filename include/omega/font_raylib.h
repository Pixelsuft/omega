#pragma once
#include <omega/api_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/font.h>
#define _RAYLIB_FONT_SIZE_MUL 1.3529f

typedef struct {
    OMG_Font parent;
    Font font;
} OMG_FontRaylib;

typedef struct {
    OMG_FontMgr parent;
} OMG_FontMgrRaylib;

OMG_API bool omg_fontmgr_raylib_init(OMG_FontMgrRaylib* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_fontmgr_font_raylib_destroy(OMG_FontMgrRaylib* this, OMG_FontRaylib* font);
OMG_API OMG_FontRaylib* omg_fontmgr_raylib_font_from_fp(OMG_FontMgrRaylib* this, OMG_FontRaylib* font, const OMG_String* fp, long index, float size);
OMG_API OMG_FontRaylib* omg_fontmgr_raylib_font_from_mem(OMG_FontMgrRaylib* this, OMG_FontRaylib* font, const void* data, size_t data_size, long index, float size);
OMG_API bool omg_fontmgr_raylib_font_query_text_size(OMG_FontMgrRaylib* this, OMG_FontRaylib* font, const OMG_String* text, OMG_FRect* size_buf);
OMG_API bool omg_fontmgr_raylib_font_set_scale(OMG_FontMgrRaylib* this, OMG_FontRaylib* font, const OMG_FPoint* scale);
#endif
#endif
