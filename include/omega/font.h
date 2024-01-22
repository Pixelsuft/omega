#pragma once
#include <omega/ostd.h>

#define OMG_FONT_MGR_NONE 0
#define OMG_FONT_MGR_SDL2 0
#define OMG_FONT_MGR_RAYLIB 0

#define OMG_FONT_TEXT_TYPE_TEXT 0
#define OMG_FONT_TEXT_TYPE_UTF8 1

typedef struct {
    float size;
    int text_type;
    bool was_allocated;
} OMG_Font;

typedef struct OMG_FontMgr {
    bool (*init)(struct OMG_FontMgr* this);
    bool (*destroy)(struct OMG_FontMgr* this);
    bool (*font_destroy)(struct OMG_FontMgr* this, OMG_Font* font);
    OMG_Font* (*font_from_fp)(struct OMG_FontMgr* this, OMG_Font* font, const OMG_String* fp, long index, float size);
    bool (*font_set_scale)(struct OMG_FontMgr* this, OMG_Font* font, const OMG_FPoint* scale);
    bool (*font_query_text_size)(struct OMG_FontMgr* this, OMG_Font* font, const OMG_String* text, OMG_FRect* size_buf);
    void* omg;
    bool was_allocated;
    bool inited;
} OMG_FontMgr;

OMG_API bool omg_fontmgr_init(OMG_FontMgr* this);
OMG_API bool omg_fontmgr_font_destroy(OMG_FontMgr* this, OMG_Font* font);
OMG_API OMG_Font* omg_fontmgr_dummy_font_create(OMG_FontMgr* this);
OMG_API OMG_Font* omg_fontmgr_font_from_fp(OMG_FontMgr* this, OMG_Font* font, const OMG_String* fp, long index, float size);
OMG_API bool omg_fontmgr_destroy(OMG_FontMgr* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_fontmgr_font_query_text_size(OMG_FontMgr* this, OMG_Font* font, const OMG_String* text, OMG_FRect* size_buf);
OMG_API bool omg_fontmgr_font_set_scale(OMG_FontMgr* this, OMG_Font* font, const OMG_FPoint* scale);
#endif
