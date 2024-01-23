#include <omega/font_sdl2.h>

#if OMG_SUPPORT_SDL2_TTF
#include <omega/omega.h>
#include <omega/omega_sdl2.h>
#define base ((OMG_FontMgr*)this)
#define omg_base ((OMG_Omega*)base->omg)
#define font_base ((OMG_Font*)font)
#define TTF_GETERROR() ((omg_base->type == OMG_OMEGA_TYPE_SDL2) ? ((OMG_OmegaSdl2*)omg_base)->sdl2->SDL_GetError() : "")

bool omg_fontmgr_sdl2_destroy(OMG_FontMgrSdl2* this) {
    if (!base->inited)
        return false;
    base->inited = false;
    this->ttf.TTF_Quit();
    return omg_sdl2_ttf_dll_free(&this->ttf);
}

bool omg_fontmgr_font_sdl2_destroy(OMG_FontMgrSdl2* this, OMG_FontSdl2* font) {
    if (OMG_ISNOTNULL(font)) {
        if (OMG_ISNOTNULL(font->font)) {
            this->ttf.TTF_CloseFont(font->font);
            font->font = NULL;
        }
        if (font_base->was_allocated) {
            font_base->was_allocated = false;
            OMG_FREE(omg_base->mem, font);
        }
    }
    return false;
}

OMG_FontSdl2* omg_fontmgr_sdl2_font_from_fp(OMG_FontMgrSdl2* this, OMG_FontSdl2* font, const OMG_String* fp, long index, float size) {
    if (omg_string_ensure_null((OMG_String*)fp))
        return (OMG_FontSdl2*)omg_fontmgr_font_from_fp(base, font_base, fp, index, size);
    if (OMG_ISNULL(font)) {
        font = OMG_MALLOC(omg_base->mem, sizeof(OMG_FontSdl2));
        if (OMG_ISNULL(font))
            return (OMG_FontSdl2*)omg_fontmgr_font_from_fp(base, font_base, fp, index, size);
        font_base->was_allocated = true;
    }
    else
        font_base->was_allocated = false;
    if (index > 0)
        font->font = this->ttf.TTF_OpenFontIndex(fp->ptr, (int)size, index);
    else
        font->font = this->ttf.TTF_OpenFont(fp->ptr, (int)size);
    if (OMG_ISNULL(font->font)) {
        _OMG_LOG_ERROR(omg_base, "Failed to open font ", fp->ptr, " (", TTF_GETERROR(), ")");
        omg_fontmgr_font_sdl2_destroy(this, font);
        return (OMG_FontSdl2*)omg_fontmgr_font_from_fp(base, font_base, fp, index, size);
    }
    font_base->text_type = OMG_FONT_TEXT_TYPE_UTF8;
    font_base->aa = true;
    font_base->size = size;
    return font;
}

bool omg_fontmgr_sdl2_font_set_scale(OMG_FontMgrSdl2* this, OMG_FontSdl2* font, const OMG_FPoint* scale) {
    if (OMG_ISNULL(this->ttf.TTF_SetFontSizeDPI)) {
        // Should I try to re-open font???
        _OMG_LOG_WARN(omg_base, "Failed to set font size (Setting size is not supported for this SDL2_ttf version)");
        return true;
    }
    if (this->ttf.TTF_SetFontSizeDPI(font->font, (int)font_base->size, (unsigned int)(scale->x * 72.0f), (unsigned int)(scale->y * 72.0f)) < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to set font size (", TTF_GETERROR(), ")");
        return true;
    }
    // TODO: work here
    return false;
}

bool omg_fontmgr_sdl2_font_query_text_size(OMG_FontMgrSdl2* this, OMG_FontSdl2* font, const OMG_String* text, OMG_FRect* size_buf) {
    if (omg_string_ensure_null((OMG_String*)text))
        return true;
    int w_buf, h_buf;
    size_buf->x = size_buf->y = 0.0f;
    if ((font_base->text_type == OMG_FONT_TEXT_TYPE_UTF8 ? this->ttf.TTF_SizeUTF8 : this->ttf.TTF_SizeText)(font->font, text->ptr, &w_buf, &h_buf) < 0) {
        _OMG_LOG_ERROR(omg_base, "Failed to query font size (", TTF_GETERROR(), ")");
        size_buf->w = size_buf->h = 0.0f;
        return true;
    }
    size_buf->w = (float)w_buf;
    size_buf->h = (float)h_buf;
    return false;
}

bool omg_fontmgr_sdl2_init(OMG_FontMgrSdl2* this) {
    omg_fontmgr_init(base);
    if (omg_sdl2_ttf_dll_load(&this->ttf, omg_base->sdl2_ttf_dll_path)) {
        _OMG_LOG_ERROR(omg_base, "Failed to load SDL2_ttf dll");
        return true;
    }
    if (this->ttf.TTF_Init() < 0) {
        _OMG_LOG_ERROR(omg_base, "Failed to load init SDL2_ttf (", TTF_GETERROR(), ")");
        omg_sdl2_ttf_dll_free(&this->ttf);
        return true;
    }
    base->type = OMG_FONT_MGR_SDL2;
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_fontmgr_sdl2_destroy;
    base->font_from_fp = omg_fontmgr_sdl2_font_from_fp;
    base->font_destroy = omg_fontmgr_font_sdl2_destroy;
    base->font_set_scale = omg_fontmgr_sdl2_font_set_scale;
    base->font_query_text_size = omg_fontmgr_sdl2_font_query_text_size;
    OMG_END_POINTER_CAST();
    base->inited = true;
    return false;
}
#endif
