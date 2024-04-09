#pragma once
#include <omega/ostd.h>
#include <omega/filesystem.h>

#define OMG_IS_DUMMY_FONT(font) ((OMG_ISNULL((font))) || ((font)->is_dummy))

#define OMG_FONT_MGR_NONE 0
#define OMG_FONT_MGR_SDL2 1
#define OMG_FONT_MGR_RAYLIB 2

#define OMG_FONT_TEXT_TYPE_TEXT 0
#define OMG_FONT_TEXT_TYPE_UTF8 1

typedef struct {
    void* extra1;
    OMG_FPoint scale;
    float a_scale;
    float size;
    float spacing;
    int text_type;
    bool wrapping;
    bool is_dummy;
    bool aa;
    bool was_allocated;
} OMG_Font;

typedef struct OMG_FontMgr {
    /* Init font manager */
    bool (*init)(struct OMG_FontMgr* this);
    /* Destroy font manager */
    bool (*destroy)(struct OMG_FontMgr* this);
    /* Destroy font */
    bool (*font_destroy)(struct OMG_FontMgr* this, OMG_Font* font);
    /* Create font from file path */
    OMG_Font* (*font_from_fp)(struct OMG_FontMgr* this, OMG_Font* font, const OMG_String* fp, long index, float size);
    /* Create font from memory */
    OMG_Font* (*font_from_mem)(struct OMG_FontMgr* this, OMG_Font* font, const void* data, size_t data_size, long index, float size);
    /* Create font from file object */
    OMG_Font* (*font_from_file)(struct OMG_FontMgr* this, OMG_Font* font, const OMG_File* file, bool destroy_file, long index, float size);
    /* Scale font */
    bool (*font_set_scale)(struct OMG_FontMgr* this, OMG_Font* font, const OMG_FPoint* scale);
    /* Query text size without rendering */
    bool (*font_query_text_size)(struct OMG_FontMgr* this, OMG_Font* font, const OMG_String* text, OMG_FRect* size_buf);
    void* omg;
    int type;
    bool was_allocated;
    bool inited;
} OMG_FontMgr;

OMG_API bool omg_fontmgr_init(OMG_FontMgr* this);
OMG_API bool omg_fontmgr_font_destroy(OMG_FontMgr* this, OMG_Font* font);
OMG_API OMG_Font* omg_fontmgr_dummy_font_create(OMG_FontMgr* this);
OMG_API OMG_Font* omg_fontmgr_font_from_fp(OMG_FontMgr* this, OMG_Font* font, const OMG_String* fp, long index, float size);
OMG_API OMG_Font* omg_fontmgr_font_from_mem(OMG_FontMgr* this, OMG_Font* font, const void* data, size_t data_size, long index, float size);
OMG_API OMG_Font* omg_fontmgr_font_from_file(OMG_FontMgr* this, OMG_Font* font, OMG_File* file, bool destroy_file, long index, float size);
OMG_API bool omg_fontmgr_destroy(OMG_FontMgr* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_fontmgr_font_query_text_size(OMG_FontMgr* this, OMG_Font* font, const OMG_String* text, OMG_FRect* size_buf);
OMG_API bool omg_fontmgr_font_set_scale(OMG_FontMgr* this, OMG_Font* font, const OMG_FPoint* scale);
#endif
