#include <omega/font_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/omega.h>
#include <omega/omega_raylib.h>
#define base ((OMG_FontMgr*)this)
#define omg_base ((OMG_Omega*)base->omg)
#define omg_raylib ((OMG_OmegaRaylib*)base->omg)
#define rl (omg_raylib->raylib)
#define font_base ((OMG_Font*)font)

bool omg_fontmgr_font_raylib_destroy(OMG_FontMgrRaylib* this, OMG_FontRaylib* font) {
    if (OMG_ISNOTNULL(font)) {
        if (rl->IsFontReady(font->font)) {
            rl->UnloadFont(font->font);
        }
        omg_fontmgr_font_destroy(base, font_base);
        if (font_base->was_allocated) {
            font_base->was_allocated = false;
            OMG_FREE(omg_base->mem, font);
        }
    }
    return false;
}

OMG_FontRaylib* omg_fontmgr_raylib_font_from_fp(OMG_FontMgrRaylib* this, OMG_FontRaylib* font, const OMG_String* fp, long index, float size) {
    if (omg_string_ensure_null((OMG_String*)fp))
        return (OMG_FontRaylib*)omg_fontmgr_font_from_fp(base, font_base, fp, index, size);
    if (OMG_ISNULL(font)) {
        font = OMG_MALLOC(omg_base->mem, sizeof(OMG_FontRaylib));
        if (OMG_ISNULL(font))
            return (OMG_FontRaylib*)omg_fontmgr_font_from_fp(base, font_base, fp, index, size);
        font_base->was_allocated = true;
    }
    else
        font_base->was_allocated = false;
    int int_sz = (int)(size * _RAYLIB_FONT_SIZE_MUL);
    font->font = rl->LoadFontEx(fp->ptr, int_sz, NULL, 0);
    if (!rl->IsFontReady(font->font)) {
        omg_fontmgr_font_destroy(base, font_base);
        _OMG_LOG_ERROR(omg_base, "Failed to load font ", fp->ptr);
        return (OMG_FontRaylib*)omg_fontmgr_font_from_fp(base, font_base, fp, index, size);
    }
    font_base->spacing = 0.0f;
    font_base->scale.x = font_base->scale.y = font_base->a_scale = 1.0f;
    font_base->text_type = OMG_FONT_TEXT_TYPE_UTF8;
    font_base->aa = true;
    font_base->extra1 = NULL;
    font_base->size = (float)int_sz / _RAYLIB_FONT_SIZE_MUL;
    return font;
}

OMG_FontRaylib* omg_fontmgr_raylib_font_from_mem(OMG_FontMgrRaylib* this, OMG_FontRaylib* font, const void* data, size_t data_size, long index, float size) {
    if (OMG_ISNULL(font)) {
        font = OMG_MALLOC(omg_base->mem, sizeof(OMG_FontRaylib));
        if (OMG_ISNULL(font))
            return (OMG_FontRaylib*)omg_fontmgr_font_from_mem(base, font_base, data, data_size, index, size);
        font_base->was_allocated = true;
    }
    else
        font_base->was_allocated = false;
    int int_sz = (int)(size * _RAYLIB_FONT_SIZE_MUL);
    font->font = rl->LoadFontFromMemory(".ttf", data, (int)data_size, int_sz, NULL, 0);
    if (!rl->IsFontReady(font->font)) {
        omg_fontmgr_font_destroy(base, font_base);
        _OMG_LOG_ERROR(omg_base, "Failed to load font from mem");
        return (OMG_FontRaylib*)omg_fontmgr_font_from_mem(base, font_base, data, data_size, index, size);
    }
    font_base->spacing = 0.0f;
    font_base->scale.x = font_base->scale.y = font_base->a_scale = 1.0f;
    font_base->text_type = OMG_FONT_TEXT_TYPE_UTF8;
    font_base->aa = true;
    font_base->extra1 = NULL;
    font_base->size = (float)int_sz / _RAYLIB_FONT_SIZE_MUL;
    return font;
}

bool omg_fontmgr_raylib_font_set_scale(OMG_FontMgrRaylib* this, OMG_FontRaylib* font, const OMG_FPoint* scale) {
    OMG_UNUSED(this, font, scale); // TODO
    return false;
}

bool omg_fontmgr_raylib_font_query_text_size(OMG_FontMgrRaylib* this, OMG_FontRaylib* font, const OMG_String* text, OMG_FRect* size_buf) {
    if (omg_string_ensure_null((OMG_String*)text))
        return true;
    Vector2 tex_size = rl->MeasureTextEx(font->font, text->ptr, font_base->size * _RAYLIB_FONT_SIZE_MUL * font_base->a_scale, font_base->spacing);
    size_buf->x = size_buf->y = 0.0f;
    size_buf->w = tex_size.x;
    size_buf->h = tex_size.y;
    return false;
}

bool omg_fontmgr_raylib_init(OMG_FontMgrRaylib* this) {
    omg_fontmgr_init(base);
    base->type = OMG_FONT_MGR_RAYLIB;
    OMG_BEGIN_POINTER_CAST();
    base->font_from_fp = omg_fontmgr_raylib_font_from_fp;
    base->font_from_mem = omg_fontmgr_raylib_font_from_mem;
    base->font_destroy = omg_fontmgr_font_raylib_destroy;
    // base->font_set_scale = omg_fontmgr_raylib_font_set_scale;
    base->font_query_text_size = omg_fontmgr_raylib_font_query_text_size;
    OMG_END_POINTER_CAST();
    base->inited = true;
    return false;
}
#endif
