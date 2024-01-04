#include <omega/renderer_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/window.h>
#include <omega/omega.h>
#include <omega/texture_raylib.h>
#define base ((OMG_Renderer*)this)
#define win_base ((OMG_Window*)base->win)
#define omg_base ((OMG_Omega*)base->omg)

void omg_renderer_raylib_update_scale(OMG_RendererRaylib* this) {
    if (!omg_base->support_highdpi)
        return;
    base->size.w = (float)this->raylib->GetRenderWidth();
    base->size.h = (float)this->raylib->GetRenderHeight();
    base->dpi_scale.x = base->size.w / (float)this->raylib->GetScreenWidth();
    base->dpi_scale.y = base->size.h / (float)this->raylib->GetScreenHeight();
}

bool omg_renderer_raylib_destroy(OMG_RendererRaylib* this) {
    if (base->inited) {
        base->inited = false;
    }
    return false;
}

bool omg_renderer_raylib_clear(OMG_RendererRaylib* this, const OMG_Color* col) {
    if (OMG_ISNULL(col))
        col = &base->color;
    this->raylib->ClearBackground(_OMG_RAYLIB_OMG_COLOR(col));
    return false;
}

bool omg_renderer_raylib_begin(OMG_RendererRaylib* this) {
    this->raylib->BeginDrawing();
    return false;
}

int omg_renderer_raylib_get_supported_drivers(OMG_RendererRaylib* this) {
    OMG_UNUSED(this);
    return OMG_REN_DRIVER_OPENGL;
}

bool omg_renderer_raylib_set_scale(OMG_RendererRaylib* this, const OMG_FPoint* offset, const OMG_FPoint* scale) {
    omg_renderer_set_scale(base, offset, scale);
    return false;
}

bool omg_renderer_raylib_set_target(OMG_RendererRaylib* this, OMG_TextureRaylib* tex) {
    if (OMG_ISNULL(tex))
        this->raylib->EndTextureMode();
    else
        this->raylib->BeginTextureMode(tex->target);
    return false;
}

bool omg_renderer_raylib_flip(OMG_RendererRaylib* this) {
    this->raylib->EndDrawing();
    return false;
}

bool omg_renderer_raylib_draw_point(OMG_RendererRaylib* this, const OMG_FPoint* pos, const OMG_Color* col) {
    return false;
}

bool omg_renderer_raylib_draw_line(OMG_RendererRaylib* this, const OMG_FRect* start_end, const OMG_Color* col) {
    return false;
}

bool omg_renderer_raylib_draw_rect(OMG_RendererRaylib* this, const OMG_FRect* rect, const OMG_Color* col) {
    return false;
}

bool omg_renderer_raylib_fill_rect(OMG_RendererRaylib* this, const OMG_FRect* rect, const OMG_Color* col) {
    return false;
}

OMG_TextureRaylib* omg_renderer_raylib_tex_create(OMG_RendererRaylib* this, const OMG_FPoint* size, int access, bool has_alpha) {
    OMG_TextureRaylib* tex = OMG_MALLOC(omg_base->mem, sizeof(OMG_TextureRaylib));
    if (OMG_ISNULL(tex))
        return NULL;
    // TODO: Does compiler use memset here???
    tex->target = this->raylib->LoadRenderTexture((int)size->w, (int)size->h);
    if (!this->raylib->IsRenderTextureReady(tex->target)) {
        OMG_FREE(omg_base->mem, tex);
        return NULL;
    }
    tex->tex = &tex->target.texture;
    return tex;
}

bool omg_renderer_raylib_tex_destroy(OMG_RendererRaylib* this, OMG_TextureRaylib* tex) {
    if (OMG_ISNULL(tex)) {
        _OMG_NULL_TEXTURE_WARN();
        return true;
    }
    if (OMG_ISNULL(tex->tex)) {
        _OMG_LOG_WARN(omg_base, "Attempted to free Raylib null texture");
        return true;
    }
    // TODO: Difference between target tex and normal tex
    this->raylib->UnloadRenderTexture(tex->target);
    tex->tex = NULL;
    OMG_FREE(omg_base->mem, tex);
    return false;
}

bool omg_renderer_raylib_init(OMG_RendererRaylib* this) {
    OMG_BEGIN_POINTER_CAST();
    omg_renderer_init(this);
    base->_on_update_window_size = omg_renderer_raylib_update_scale;
    base->destroy = omg_renderer_raylib_destroy;
    base->clear = omg_renderer_raylib_clear;
    base->begin = omg_renderer_raylib_begin;
    base->flip = omg_renderer_raylib_flip;
    base->set_scale = omg_renderer_raylib_set_scale;
    base->set_target = omg_renderer_raylib_set_target;
    base->draw_point = omg_renderer_raylib_draw_rect;
    base->draw_line = omg_renderer_raylib_draw_line;
    base->draw_rect = omg_renderer_raylib_draw_rect;
    base->fill_rect = omg_renderer_raylib_fill_rect;
    base->tex_create = omg_renderer_raylib_tex_create;
    base->tex_destroy = omg_renderer_raylib_tex_destroy;
    OMG_END_POINTER_CAST();
    base->type = OMG_REN_TYPE_RAYLIB;
    base->inited = true;
    _OMG_LOG_INFO(omg_base, "Raylib renderer created successfuly with opengl driver");
    return false;
}
#endif
