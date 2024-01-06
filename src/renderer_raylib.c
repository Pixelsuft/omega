#include <omega/renderer_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/window.h>
#include <omega/omega.h>
#include <omega/surface_raylib.h>
#include <omega/texture_raylib.h>
#define base ((OMG_Renderer*)this)
#define tex_base ((OMG_Texture*)tex)
#define win_base ((OMG_Window*)base->win)
#define omg_base ((OMG_Omega*)base->omg)
#define IS_DEFAULT_SCALE() ((base->scale.x == 1.0f) && (base->scale.y == 1.0f) && (base->offset.x == 0.0f) && (base->offset.y == 0.0f))
#define RAYLIB_HAS_SS() ((this->ss.x != 1.0f) || (this->ss.y != 1.0f))

void omg_renderer_raylib_update_scale(OMG_RendererRaylib* this) {
    // if (!omg_base->support_highdpi)
    //     return;
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

int omg_renderer_raylib_get_supported_drivers(OMG_RendererRaylib* this) {
    OMG_UNUSED(this);
    return OMG_REN_DRIVER_OPENGL;
}

bool omg_renderer_raylib_set_scale(OMG_RendererRaylib* this, const OMG_FPoint* offset, const OMG_FPoint* scale) {
    omg_renderer_set_scale(base, offset, scale);
    if (IS_DEFAULT_SCALE()) {
        this->ss.x = this->ss.y = 1.0f;
        this->raylib->EndMode2D();
    }
    else {
        Camera2D cam;
        if (base->soft_scale) {
            cam.offset.x = base->offset.x * base->scale.x;
            cam.offset.y = base->offset.y * base->scale.y;
            cam.zoom = 1.0f;
            this->ss.x = base->scale.x;
            this->ss.y = base->scale.y;
        }
        else {
            cam.offset.x = base->offset.x * base->scale.x;
            cam.offset.y = base->offset.y * base->scale.y;
            cam.zoom = base->a_scale;
            this->ss.x = this->ss.y = 1.0f;
        }
        cam.target.x = cam.target.y = 0.0f;
        cam.rotation = 0.0f;
        this->raylib->BeginMode2D(cam);
    }
    return false;
}

bool omg_renderer_raylib_begin(OMG_RendererRaylib* this) {
    this->raylib->BeginDrawing();
    if (!IS_DEFAULT_SCALE())
        omg_renderer_raylib_set_scale(this, NULL, NULL);
    return false;
}

bool omg_renderer_raylib_set_target(OMG_RendererRaylib* this, OMG_TextureRaylib* tex) {
    if (OMG_ISNULL(tex))
        this->raylib->EndTextureMode();
    else
        this->raylib->BeginTextureMode(tex->target);
    base->target = tex_base;
    omg_renderer_raylib_set_scale(this, NULL, NULL); // WTF
    return false;
}

bool omg_renderer_raylib_flip(OMG_RendererRaylib* this) {
    this->raylib->EndDrawing();
    return false;
}

bool omg_renderer_raylib_draw_line(OMG_RendererRaylib* this, const OMG_FRect* start_end, const OMG_Color* col) {
    Vector2 vec1 = { .x = start_end->x1 * this->ss.x, .y = start_end->y1 * this->ss.y };
    Vector2 vec2 = { .x = start_end->x2 * this->ss.x, .y = start_end->y2 * this->ss.y };
    this->raylib->DrawLineV(vec1, vec2, _OMG_RAYLIB_OMG_COLOR(col));
    return false;
}

bool omg_renderer_raylib_draw_rect(OMG_RendererRaylib* this, const OMG_FRect* rect, const OMG_Color* col) {
    RL_Rectangle rec = { .x = rect->x * this->ss.x, .y = rect->y * this->ss.y, .width = rect->w * this->ss.x, .height = rect->h * this->ss.y };
    this->raylib->DrawRectangleLinesEx(rec, 1.0f, _OMG_RAYLIB_OMG_COLOR(col));
    return false;
}

bool omg_renderer_raylib_fill_rect(OMG_RendererRaylib* this, const OMG_FRect* rect, const OMG_Color* col) {
    RL_Rectangle rec = { .x = rect->x * this->ss.x, .y = rect->y * this->ss.y, .width = rect->w * this->ss.x, .height = rect->h * this->ss.y };
    this->raylib->DrawRectangleRec(rec, _OMG_RAYLIB_OMG_COLOR(col));
    return false;
}

bool omg_renderer_raylib_draw_point(OMG_RendererRaylib* this, const OMG_FPoint* pos, const OMG_Color* col) {
    if (RAYLIB_HAS_SS()) {
        RL_Rectangle rec = { .x = pos->x * this->ss.x, .y = pos->y * this->ss.y, .width = this->ss.x, .height = this->ss.y };
        this->raylib->DrawRectangleRec(rec, _OMG_RAYLIB_OMG_COLOR(col));
        return false;
    }
    Vector2 vec = { .x = pos->x * this->ss.x, .y = pos->y * this->ss.y };
    this->raylib->DrawPixelV(vec, _OMG_RAYLIB_OMG_COLOR(col));
    return false;
}

bool omg_renderer_raylib_draw_circle(OMG_RendererRaylib* this, const OMG_FPoint* pos, float rad, const OMG_Color* col) {
    if (RAYLIB_HAS_SS())
        this->raylib->DrawEllipseLines(
            (int)(pos->x * this->ss.x),
            (int)(pos->y * this->ss.y),
            rad * this->ss.x,
            rad * this->ss.y,
            _OMG_RAYLIB_OMG_COLOR(col)
        );
    else {
        Vector2 vec = { .x = pos->x, .y = pos->y };
        this->raylib->DrawCircleLinesV(vec, rad, _OMG_RAYLIB_OMG_COLOR(col));
    }
    return false;
}

bool omg_renderer_raylib_fill_circle(OMG_RendererRaylib* this, const OMG_FPoint* pos, float rad, const OMG_Color* col) {
    if (RAYLIB_HAS_SS())
        this->raylib->DrawEllipse(
            (int)(pos->x * this->ss.x),
            (int)(pos->y * this->ss.y),
            rad * this->ss.x,
            rad * this->ss.y,
            _OMG_RAYLIB_OMG_COLOR(col)
        );
    else {
        Vector2 vec = { .x = pos->x, .y = pos->y };
        this->raylib->DrawCircleV(vec, rad, _OMG_RAYLIB_OMG_COLOR(col));
    }
    return false;
}

OMG_TextureRaylib* omg_renderer_raylib_tex_from_surf(OMG_RendererRaylib* this, OMG_SurfaceRaylib* surf, bool destroy_surf) {
    if (OMG_ISNULL(surf) || !this->raylib->IsImageReady(surf->img))
        return NULL;
    OMG_TextureRaylib* tex = OMG_MALLOC(omg_base->mem, sizeof(OMG_TextureRaylib));
    if (OMG_ISNULL(tex))
        return NULL;
    tex->target.texture = this->raylib->LoadTextureFromImage(surf->img);
    tex->tex = &tex->target.texture;
    if (!this->raylib->IsTextureReady(*tex->tex)) {
        OMG_FREE(omg_base->mem, tex);
        _OMG_LOG_ERROR(omg_base, "Failed to create Raylib texture from surface");
        return NULL;
    }
    tex->is_target = false;
    tex->tint.r = tex->tint.g = tex->tint.b = tex->tint.a = 255;
    tex_base->has_alpha = surf->parent.has_alpha;
    tex_base->auto_blend = true;
    tex_base->size.w = surf->parent.size.w;
    tex_base->size.h = surf->parent.size.h;
    if (destroy_surf)
        omg_base->winmgr->surf_destroy(omg_base->winmgr, (OMG_Surface*)surf);
    return tex;
}

OMG_TextureRaylib* omg_renderer_raylib_tex_create(OMG_RendererRaylib* this, const OMG_FPoint* size, int access, bool has_alpha) {
    OMG_UNUSED(access, has_alpha);
    OMG_TextureRaylib* tex = OMG_MALLOC(omg_base->mem, sizeof(OMG_TextureRaylib));
    if (OMG_ISNULL(tex))
        return NULL;
    // TODO: Does compiler use memset here???
    tex->target = this->raylib->LoadRenderTexture((int)size->w, (int)size->h);
    if (!this->raylib->IsRenderTextureReady(tex->target)) {
        OMG_FREE(omg_base->mem, tex);
        _OMG_LOG_ERROR(omg_base, "Failed to create Raylib target texture");
        return NULL;
    }
    tex->tex = &tex->target.texture;
    tex->is_target = true;
    tex->tint.r = tex->tint.g = tex->tint.b = tex->tint.a = 255;
    tex_base->has_alpha = true;
    tex_base->auto_blend = true;
    tex_base->size.w = size->w;
    tex_base->size.h = size->h;
    return tex;
}

bool omg_renderer_raylib_tex_destroy(OMG_RendererRaylib* this, OMG_TextureRaylib* tex) {
    if (OMG_ISNULL(tex)) {
        _OMG_NULL_TEXTURE_WARN();
        return true;
    }
    if (OMG_ISNULL(tex->tex) || !(tex->is_target ? this->raylib->IsRenderTextureReady(tex->target) : this->raylib->IsTextureReady(*tex->tex))) {
        _OMG_LOG_WARN(omg_base, "Attempted to free Raylib null texture");
        return true;
    }
    if (tex->is_target)
        this->raylib->UnloadRenderTexture(tex->target);
    else
        this->raylib->UnloadTexture(*tex->tex);
    tex->tex = NULL;
    OMG_FREE(omg_base->mem, tex);
    return false;
}

bool omg_renderer_raylib_copy(OMG_RendererRaylib* this, OMG_TextureRaylib* tex, const OMG_FPoint* pos) {
    if (RAYLIB_HAS_SS()) {
        RL_Rectangle src = { .x = 0.0f, .y = 0.0f, .width = tex_base->size.w, .height = tex_base->size.h };
        RL_Rectangle dst;
        if (OMG_ISNULL(pos)) {
            dst.x = dst.y = 0.0f;
        }
        else {
            dst.x = pos->x * this->ss.x;
            dst.y = pos->y * this->ss.y;
        }
        dst.width = tex_base->size.w * this->ss.x;
        dst.height = tex_base->size.h * this->ss.y;
        Vector2 origin = { .x = 0.0f, .y = 0.0f };
        this->raylib->DrawTexturePro(*tex->tex, src, dst, origin, 0.0f, tex->tint);
    }
    else {
        Vector2 vec;
        if (OMG_ISNULL(pos)) {
            vec.x = vec.y = 0.0f;
        }
        else {
            vec.x = pos->x * this->ss.x;
            vec.y = pos->y * this->ss.y;
        }
        this->raylib->DrawTextureV(*tex->tex, vec, tex->tint);
    }
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
    base->draw_point = omg_renderer_raylib_draw_point;
    base->draw_line = omg_renderer_raylib_draw_line;
    base->draw_rect = omg_renderer_raylib_draw_rect;
    base->fill_rect = omg_renderer_raylib_fill_rect;
    base->draw_circle = omg_renderer_raylib_draw_circle;
    base->fill_circle = omg_renderer_raylib_fill_circle;
    base->tex_from_surf = omg_renderer_raylib_tex_from_surf;
    base->tex_create = omg_renderer_raylib_tex_create;
    base->tex_destroy = omg_renderer_raylib_tex_destroy;
    base->copy = omg_renderer_raylib_copy;
    OMG_END_POINTER_CAST();
    this->ss.x = this->ss.y = 1.0f;
    base->type = OMG_REN_TYPE_RAYLIB;
    base->inited = true;
    omg_renderer_raylib_update_scale(this);
    _OMG_LOG_INFO(omg_base, "Raylib renderer created successfuly with opengl driver");
    return false;
}
#endif
