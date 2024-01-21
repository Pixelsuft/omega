#include <omega/config.h>
#include <omega/font.h>

bool omg_fontmgr_destroy(OMG_FontMgr* this) {
    this->inited = false;
    return false;
}

OMG_Font* omg_fontmgr_dummy_font_create(OMG_FontMgr* this) {
    OMG_UNUSED(this);
#if OMG_ALLOW_DUMMY_FONT
    static OMG_Font font;
    return &font;
#else
    return NULL;
#endif
}

bool omg_fontmgr_font_destroy(OMG_FontMgr* this, OMG_Font* font) {
    OMG_UNUSED(this, font);
    return false;
}

OMG_Font* omg_fontmgr_font_from_fp(OMG_FontMgr* this, OMG_Font* font, const OMG_String* fp, float size) {
    OMG_UNUSED(font, fp, size);
    return omg_fontmgr_dummy_font_create(this);
}

bool omg_fontmgr_init(OMG_FontMgr* this) {
    this->inited = false;
    this->destroy = omg_fontmgr_destroy;
    this->font_from_fp = omg_fontmgr_font_from_fp;
    this->font_destroy = omg_fontmgr_font_destroy;
    return false;
}
