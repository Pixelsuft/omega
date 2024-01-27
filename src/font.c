#include <omega/config.h>
#include <omega/font.h>
#include <omega/omega.h>

#define omg_base ((OMG_Omega*)this->omg)

bool omg_fontmgr_destroy(OMG_FontMgr* this) {
    this->inited = false;
    return false;
}

OMG_Font* omg_fontmgr_dummy_font_create(OMG_FontMgr* this) {
    OMG_UNUSED(this);
#if OMG_ALLOW_DUMMY_FONT
    static OMG_Font font;
    font.size = 0.0f;
    font.aa = false;
    font.spacing = 0.0f;
    font.text_type = OMG_FONT_TEXT_TYPE_TEXT;
    return &font;
#else
    return NULL;
#endif
}

bool omg_fontmgr_font_destroy(OMG_FontMgr* this, OMG_Font* font) {
    OMG_UNUSED(this, font);
    return false;
}

OMG_Font* omg_fontmgr_font_from_fp(OMG_FontMgr* this, OMG_Font* font, const OMG_String* fp, long index, float size) {
    OMG_UNUSED(font, fp, index, size);
    return omg_fontmgr_dummy_font_create(this);
}

OMG_Font* omg_fontmgr_font_from_mem(OMG_FontMgr* this, OMG_Font* font, const void* data, size_t data_size, long index, float size) {
    OMG_UNUSED(this, font, data, data_size, index, size);
    return omg_fontmgr_dummy_font_create(this);
}

OMG_Font* omg_fontmgr_font_from_file(OMG_FontMgr* this, OMG_Font* font, OMG_File* file, bool destroy_file, long index, float size) {
    if (OMG_ISNULL(file)) {
        _OMG_LOG_WARN(omg_base, "Null pointer passed for creating font from file");
        return omg_fontmgr_dummy_font_create(this);
    }
    int64_t file_size = file->get_size(file);
    if (file_size <= 0) {
        if (destroy_file)
            file->destroy(file);
        return omg_fontmgr_dummy_font_create(this);
    }
    void* buf = OMG_MALLOC(omg_base->mem, file_size);
    if (OMG_ISNULL(buf)) {
        if (destroy_file)
            file->destroy(file);
        return omg_fontmgr_dummy_font_create(this);
    }
    size_t size_read = file->read(file, buf, 1, (size_t)file_size);
    if (destroy_file)
        file->destroy(file);
    if (size_read == 0) {
        OMG_FREE(omg_base->mem, buf);
        return omg_fontmgr_dummy_font_create(this);
    }
    OMG_Font* res = this->font_from_mem(this, font, buf, size_read, index, size);
    OMG_FREE(omg_base->mem, buf);
    return res;
}

bool omg_fontmgr_font_set_scale(OMG_FontMgr* this, OMG_Font* font, const OMG_FPoint* scale) {
    OMG_UNUSED(this);
    font->scale.x = scale->x;
    font->scale.y = scale->y;
    font->a_scale = (scale->x + scale->y) / 2.0f;
    return false;
}

bool omg_fontmgr_font_query_text_size(OMG_FontMgr* this, OMG_Font* font, const OMG_String* text, OMG_FRect* size_buf) {
    OMG_UNUSED(this, font, text);
    size_buf->x = size_buf->y = 0.0f;
    size_buf->w = size_buf->h = 0.0f;
    return true;
}

bool omg_fontmgr_init(OMG_FontMgr* this) {
    this->inited = false;
    this->type = OMG_FONT_MGR_NONE;
    this->destroy = omg_fontmgr_destroy;
    this->font_from_fp = omg_fontmgr_font_from_fp;
    this->font_from_mem = omg_fontmgr_font_from_mem;
    OMG_BEGIN_POINTER_CAST();
    this->font_from_file = omg_fontmgr_font_from_file;
    OMG_END_POINTER_CAST();
    this->font_destroy = omg_fontmgr_font_destroy;
    this->font_set_scale = omg_fontmgr_font_set_scale;
    this->font_query_text_size = omg_fontmgr_font_query_text_size;
    return false;
}
