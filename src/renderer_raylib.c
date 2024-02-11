#include <omega/renderer_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/window.h>
#include <omega/omega.h>
#include <omega/surface_raylib.h>
#include <omega/texture_raylib.h>
#include <omega/font_raylib.h>
#define base ((OMG_Renderer*)this)
#define tex_base ((OMG_Texture*)tex)
#define win_base ((OMG_Window*)base->win)
#define omg_base ((OMG_Omega*)base->omg)
#define font_raylib ((OMG_FontRaylib*)font)
#define _RL_THICK_DIV 2.0f
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
        this->ss.x = this->ss.y = this->ss.w = 1.0f;
        this->so.x = this->so.y = 0.0f;
        this->raylib->EndMode2D();
    }
    else {
        Camera2D cam;
        if (base->soft_scale) {
            if (base->soft_offset) {
                this->so.x = base->offset.x;
                this->so.y = base->offset.y;
                cam.offset.x = cam.offset.y = 0.0f;
            }
            else {
                cam.offset.x = base->offset.x * base->scale.x;
                cam.offset.y = base->offset.y * base->scale.y;
                this->so.x = this->so.y = 0.0f;
            }
            cam.zoom = 1.0f;
            this->ss.x = base->scale.x;
            this->ss.y = base->scale.y;
        }
        else {
            if (base->soft_offset) {
                this->so.x = base->offset.x;
                this->so.y = base->offset.y;
                cam.offset.x = cam.offset.y = 0.0f;
            }
            else {
                cam.offset.x = base->offset.x * base->scale.x;
                cam.offset.y = base->offset.y * base->scale.y;
                this->so.x = this->so.y = 0.0f;
            }
            cam.zoom = base->a_scale;
            this->ss.x = this->ss.y = 1.0f;
        }
        cam.target.x = cam.target.y = 0.0f;
        cam.rotation = 0.0f;
        if (base->soft_scale && base->soft_offset)
            this->raylib->EndMode2D();
        else
            this->raylib->BeginMode2D(cam);
    }
    this->ss.w = (this->ss.x + this->ss.y) / 2.0f;
    return false;
}

bool omg_renderer_raylib_begin(OMG_RendererRaylib* this) {
    this->raylib->BeginDrawing();
    if (!IS_DEFAULT_SCALE())
        omg_renderer_raylib_set_scale(this, NULL, NULL);
    return false;
}

bool omg_renderer_raylib_set_target(OMG_RendererRaylib* this, OMG_TextureRaylib* tex) {
    if (OMG_IS_DUMMY_TEX(tex_base))
        this->raylib->EndTextureMode();
    else
        this->raylib->BeginTextureMode(tex->target);
    base->target = tex_base;
    omg_renderer_raylib_set_scale(this, NULL, NULL);
    return false;
}

bool omg_renderer_raylib_flip(OMG_RendererRaylib* this) {
    this->raylib->EndDrawing();
    return false;
}

bool omg_renderer_raylib_draw_line(OMG_RendererRaylib* this, const OMG_FRect* start_end, const OMG_Color* col) {
    Vector2 vec1 = { .x = (start_end->x1 + this->so.x) * this->ss.x, .y = (start_end->y1 + this->so.y) * this->ss.y };
    Vector2 vec2 = { .x = (start_end->x2 + this->so.x) * this->ss.x, .y = (start_end->y2 + this->so.y) * this->ss.y };
    this->raylib->DrawLineV(vec1, vec2, _OMG_RAYLIB_OMG_COLOR(col));
    return false;
}

bool omg_renderer_raylib_draw_rect(OMG_RendererRaylib* this, const OMG_FRect* rect, const OMG_Color* col) {
    RL_Rectangle rec = {
        .x = (rect->x + this->so.x) * this->ss.x, .y = (rect->y + this->so.y) * this->ss.y,
        .width = rect->w * this->ss.x, .height = rect->h * this->ss.y
    };
    this->raylib->DrawRectangleLinesEx(rec, 1.0f, _OMG_RAYLIB_OMG_COLOR(col));
    return false;
}

bool omg_renderer_raylib_fill_rect(OMG_RendererRaylib* this, const OMG_FRect* rect, const OMG_Color* col) {
    RL_Rectangle rec = {
        .x = (rect->x + this->so.x) * this->ss.x, .y = (rect->y + this->so.y) * this->ss.y,
        .width = rect->w * this->ss.x, .height = rect->h * this->ss.y
    };
    this->raylib->DrawRectangleRec(rec, _OMG_RAYLIB_OMG_COLOR(col));
    return false;
}

bool omg_renderer_raylib_draw_point(OMG_RendererRaylib* this, const OMG_FPoint* pos, const OMG_Color* col) {
    if (RAYLIB_HAS_SS()) {
        RL_Rectangle rec = {
            .x = (pos->x + this->so.x) * this->ss.x, .y = (pos->y + this->so.y) * this->ss.y,
            .width = this->ss.x, .height = this->ss.y
        };
        this->raylib->DrawRectangleRec(rec, _OMG_RAYLIB_OMG_COLOR(col));
        return false;
    }
    Vector2 vec = { .x = pos->x + this->so.x, .y = pos->y + this->so.y };
    this->raylib->DrawPixelV(vec, _OMG_RAYLIB_OMG_COLOR(col));
    return false;
}

bool omg_renderer_raylib_draw_circle(OMG_RendererRaylib* this, const OMG_FPoint* pos, float rad, const OMG_Color* col) {
    if (RAYLIB_HAS_SS())
        this->raylib->DrawEllipseLines(
            (int)((pos->x + this->so.x) * this->ss.x),
            (int)((pos->y + this->so.y) * this->ss.y),
            rad * this->ss.x,
            rad * this->ss.y,
            _OMG_RAYLIB_OMG_COLOR(col)
        );
    else {
        Vector2 vec = { .x = pos->x + this->so.x, .y = pos->y + this->so.y };
        this->raylib->DrawCircleLinesV(vec, rad, _OMG_RAYLIB_OMG_COLOR(col));
    }
    return false;
}

bool omg_renderer_raylib_draw_ellipse(OMG_RendererRaylib* this, const OMG_FPoint* pos, float rx, float ry, const OMG_Color* col) {
    this->raylib->DrawEllipseLines(
        (int)((pos->x + this->so.x) * this->ss.x),
        (int)((pos->y + this->so.y) * this->ss.y),
        rx * this->ss.x,
        ry * this->ss.y,
        _OMG_RAYLIB_OMG_COLOR(col)
    );
    return false;
}

bool omg_renderer_raylib_fill_ellipse(OMG_RendererRaylib* this, const OMG_FPoint* pos, float rx, float ry, const OMG_Color* col) {
    this->raylib->DrawEllipse(
        (int)((pos->x + this->so.x) * this->ss.x),
        (int)((pos->y + this->so.y) * this->ss.y),
        rx * this->ss.x,
        ry * this->ss.y,
        _OMG_RAYLIB_OMG_COLOR(col)
    );
    return false;
}

bool omg_renderer_raylib_fill_circle(OMG_RendererRaylib* this, const OMG_FPoint* pos, float rad, const OMG_Color* col) {
    if (RAYLIB_HAS_SS())
        this->raylib->DrawEllipse(
            (int)((pos->x + this->so.x) * this->ss.x),
            (int)((pos->y + this->so.y) * this->ss.y),
            rad * this->ss.x,
            rad * this->ss.y,
            _OMG_RAYLIB_OMG_COLOR(col)
        );
    else {
        Vector2 vec = { .x = pos->x + this->so.x, .y = pos->y + this->so.y };
        this->raylib->DrawCircleV(vec, rad, _OMG_RAYLIB_OMG_COLOR(col));
    }
    return false;
}

bool omg_renderer_raylib_tex_set_scale_mode(OMG_RendererRaylib* this, OMG_TextureRaylib* tex, int scale_mode) {
    _OMG_NULL_TEXTURE_CHECK(tex_base);
    this->raylib->SetTextureFilter(*tex->tex, (
        (scale_mode == OMG_SCALE_MODE_LINEAR) ? TEXTURE_FILTER_BILINEAR : (scale_mode == OMG_SCALE_MODE_NEAREST ? TEXTURE_FILTER_POINT : TEXTURE_FILTER_BILINEAR)
    ));
    return false;
}

OMG_TextureRaylib* omg_renderer_raylib_tex_from_surf(OMG_RendererRaylib* this, OMG_TextureRaylib* tex, OMG_SurfaceRaylib* surf, bool destroy_surf) {
    if (OMG_ISNULL(surf) || !this->raylib->IsImageReady(surf->img))
        return (OMG_TextureRaylib*)omg_renderer_tex_from_surf(base, tex_base, (OMG_Surface*)surf, destroy_surf);
    if (OMG_ISNULL(tex)) {
        tex = OMG_MALLOC(omg_base->mem, sizeof(OMG_TextureRaylib));
        if (OMG_ISNULL(tex))
            return (OMG_TextureRaylib*)omg_renderer_tex_from_surf(base, tex_base, (OMG_Surface*)surf, destroy_surf);
#if OMG_ALLOW_TEX_WAS_ALLOCATED
        tex_base->was_allocated = true;
#endif
    }
#if OMG_ALLOW_TEX_WAS_ALLOCATED
    else
        tex_base->was_allocated = false;
#endif
    tex->target.texture = this->raylib->LoadTextureFromImage(surf->img);
    tex->tex = &tex->target.texture;
    if (!this->raylib->IsTextureReady(*tex->tex)) {
        OMG_FREE(omg_base->mem, tex);
        _OMG_LOG_ERROR(omg_base, "Failed to create Raylib texture from surface");
        return (OMG_TextureRaylib*)omg_renderer_tex_from_surf(base, tex_base, (OMG_Surface*)surf, destroy_surf);
    }
    tex->is_target = false;
    tex->tint.r = tex->tint.g = tex->tint.b = tex->tint.a = 255;
    tex_base->has_alpha = surf->parent.has_alpha;
    tex_base->size.w = surf->parent.size.w;
    tex_base->size.h = surf->parent.size.h;
    if (destroy_surf)
        omg_base->winmgr->surf_destroy(omg_base->winmgr, (OMG_Surface*)surf);
    return tex;
}

OMG_TextureRaylib* omg_renderer_raylib_tex_create(OMG_RendererRaylib* this, OMG_TextureRaylib* tex, const OMG_FPoint* size, int access, bool has_alpha) {
    OMG_UNUSED(access, has_alpha);
    if (OMG_ISNULL(tex)) {
        tex = OMG_MALLOC(omg_base->mem, sizeof(OMG_TextureRaylib));
        if (OMG_ISNULL(tex))
            return (OMG_TextureRaylib*)omg_renderer_tex_create(base, tex_base, size, access, has_alpha);
#if OMG_ALLOW_TEX_WAS_ALLOCATED
        tex_base->was_allocated = true;
#endif
    }
#if OMG_ALLOW_TEX_WAS_ALLOCATED
    else
        tex_base->was_allocated = false;
#endif
    // TODO: Does compiler use memset here???
    tex->target = this->raylib->LoadRenderTexture((int)size->w, (int)size->h);
    if (!this->raylib->IsRenderTextureReady(tex->target)) {
        OMG_FREE(omg_base->mem, tex);
        _OMG_LOG_ERROR(omg_base, "Failed to create Raylib target texture");
        return (OMG_TextureRaylib*)omg_renderer_tex_create(base, tex_base, size, access, has_alpha);
    }
    tex->tex = &tex->target.texture;
    tex->is_target = true;
    tex->tint.r = tex->tint.g = tex->tint.b = tex->tint.a = 255;
    tex_base->has_alpha = true;
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
#if OMG_ALLOW_TEX_WAS_ALLOCATED
    if (tex_base->was_allocated)
#endif
    OMG_FREE(omg_base->mem, tex);
    return false;
}

bool omg_renderer_raylib_copy(OMG_RendererRaylib* this, OMG_TextureRaylib* tex, const OMG_FPoint* pos) {
    _OMG_NULL_TEXTURE_CHECK(tex_base);
    if (RAYLIB_HAS_SS() || tex->is_target) {
        RL_Rectangle src = { .x = 0.0f, .y = 0.0f, .width = tex_base->size.w, .height = tex_base->size.h };
        if (tex->is_target)
            src.height = -src.height; // Fuck Raylib
        RL_Rectangle dst;
        if (OMG_ISNULL(pos)) {
            dst.x = this->so.x;
            dst.y = this->so.y;
        }
        else {
            dst.x = (pos->x + this->so.x) * this->ss.x;
            dst.y = (pos->y + this->so.y) * this->ss.y;
        }
        dst.width = tex_base->size.w * this->ss.x;
        dst.height = tex_base->size.h * this->ss.y;
        Vector2 origin = { .x = 0.0f, .y = 0.0f };
        this->raylib->DrawTexturePro(*tex->tex, src, dst, origin, 0.0f, tex->tint);
    }
    else {
        Vector2 vec;
        if (OMG_ISNULL(pos)) {
            vec.x = this->so.x;
            vec.y = this->so.y;
        }
        else {
            vec.x = pos->x + this->so.x;
            vec.y = pos->y + this->so.y;
        }
        this->raylib->DrawTextureV(*tex->tex, vec, tex->tint);
    }
    return false;
}

bool omg_renderer_raylib_copy_ex(OMG_RendererRaylib* this, OMG_TextureRaylib* tex, const OMG_FRect* src, const OMG_FRect* dst, const OMG_FPoint* origin, const double rot) {
    _OMG_NULL_TEXTURE_CHECK(tex_base);
    RL_Rectangle src_rect;
    RL_Rectangle dst_rect;
    Vector2 origin_vec;
    if (OMG_ISNULL(src)) {
        src_rect.x = 0;
        src_rect.y = 0;
        src_rect.width = tex_base->size.w;
        src_rect.height = tex_base->size.h;
    }
    else {
        src_rect.x = src->x;
        src_rect.y = src->y;
        src_rect.width = src->w;
        src_rect.height = src->h;
        if (src_rect.width == 0.0f)
            src_rect.width = tex_base->size.w;
        if (src_rect.height == 0.0f)
            src_rect.height = tex_base->size.h;
    }
    if (OMG_ISNULL(dst)) {
        dst_rect.x = base->offset.x * this->ss.x;
        dst_rect.y = base->offset.y * this->ss.y;
        dst_rect.width = tex_base->size.w * this->ss.x;
        dst_rect.height = tex_base->size.h * this->ss.y;
    } {
        dst_rect.x = (dst->x + base->offset.x) * this->ss.x;
        dst_rect.y = (dst->y + base->offset.y) * this->ss.y;
        dst_rect.width = dst->w * this->ss.x;
        dst_rect.height = dst->h * this->ss.y;
        if (dst_rect.width == 0.0f)
            dst_rect.width = tex_base->size.w * this->ss.x;
        if (dst_rect.height == 0.0f)
            dst_rect.height = tex_base->size.h * this->ss.y;
    }
    if (OMG_ISNULL(origin)) {
        origin_vec.x = dst_rect.width / 2.0f;
        origin_vec.y = dst_rect.height / 2.0f;
    }
    else {
        origin_vec.x = origin->x * this->ss.x;
        origin_vec.y = origin->y * this->ss.y;
    }
    this->raylib->DrawTexturePro(
        *tex->tex, src_rect, dst_rect, origin_vec,
#if OMG_USE_RADIANS
        (float)(rot * 180.0 / OMG_M_PI),
#else
        (float)rot,
#endif
        tex->tint
    );
    return false;
}

bool omg_renderer_raylib_tex_set_color_mod(OMG_RendererRaylib* this, OMG_TextureRaylib* tex, const OMG_Color* col) {
    _OMG_NULL_TEXTURE_CHECK(tex_base);
    OMG_UNUSED(this);
    tex->tint = _OMG_RAYLIB_OMG_COLOR(col);
    return false;
}

bool omg_renderer_raylib_set_blend_mode(OMG_RendererRaylib* this, int blend_mode) {
    if ((blend_mode == OMG_BLEND_MODE_NONE) || (blend_mode == OMG_BLEND_MODE_BLEND)) {
        this->raylib->EndBlendMode();
        return false;
    }
    int blend = BLEND_ALPHA;
    if (blend_mode == OMG_BLEND_MODE_ADD)
        blend = BLEND_ADD_COLORS;
    else if (blend_mode == OMG_BLEND_MODE_ADDDITIVE)
        blend = BLEND_ADDITIVE;
    else if (blend_mode == OMG_BLEND_MODE_SUB)
        blend = BLEND_SUBTRACT_COLORS;
    else if (blend_mode == OMG_BLEND_MODE_MUL)
        blend = BLEND_MULTIPLIED;
    this->blend_cache = blend;
    this->raylib->BeginBlendMode(blend);
    return false;
}

bool omg_renderer_raylib_set_vsync(OMG_RendererRaylib* this, bool enabled) {
    win_base->vsync = enabled;
    if (enabled)
        this->raylib->SetWindowState(FLAG_VSYNC_HINT);
    else
        this->raylib->ClearWindowState(FLAG_VSYNC_HINT);
    return false;
}

bool omg_renderer_raylib_set_clip_rect(OMG_RendererRaylib* this, const OMG_FRect* clip_rect) {
    if (OMG_ISNULL(clip_rect))
        this->raylib->EndScissorMode();
    else
        this->raylib->BeginScissorMode((int)clip_rect->x, (int)clip_rect->y, (int)clip_rect->w, (int)clip_rect->h);
    return false;
}

OMG_TextureRaylib* omg_renderer_raylib_font_render(OMG_RendererRaylib* this, OMG_TextureRaylib* tex, OMG_Font* font, const OMG_String* text, const OMG_Color* bg, const OMG_Color* fg, OMG_FRect* rect) {
    if (OMG_IS_DUMMY_FONT(font) || omg_string_ensure_null((OMG_String*)text))
        return (OMG_TextureRaylib*)omg_renderer_font_render(base, tex_base, font, text, bg, fg, rect);
    if (OMG_ISNULL(tex)) {
        tex = OMG_MALLOC(omg_base->mem, sizeof(OMG_TextureRaylib));
        if (OMG_ISNULL(tex))
            return (OMG_TextureRaylib*)omg_renderer_font_render(base, tex_base, font, text, bg, fg, rect);
#if OMG_ALLOW_TEX_WAS_ALLOCATED
        tex_base->was_allocated = true;
#endif
    }
#if OMG_ALLOW_TEX_WAS_ALLOCATED
    else
        tex_base->was_allocated = false;
#endif
    Vector2 tex_size = this->raylib->MeasureTextEx(font_raylib->font, text->ptr, font->size * _RAYLIB_FONT_SIZE_MUL * font->a_scale, font->spacing);
    tex->target = this->raylib->LoadRenderTexture((int)tex_size.x, (int)tex_size.y);
    if (!this->raylib->IsRenderTextureReady(tex->target)) {
        OMG_FREE(omg_base->mem, tex);
        _OMG_LOG_ERROR(omg_base, "Failed to create Raylib font texture from surf");
        return (OMG_TextureRaylib*)omg_renderer_font_render(base, tex_base, font, text, bg, fg, rect);
    }
    tex->tex = &tex->target.texture;
    tex->is_target = true;
    tex->tint.r = tex->tint.g = tex->tint.b = tex->tint.a = 255;
    tex_base->has_alpha = true;
    tex_base->size.w = tex_size.x;
    tex_base->size.h = tex_size.y;
    OMG_TextureRaylib* target_cache = (OMG_TextureRaylib*)base->target;
    this->raylib->BeginTextureMode(tex->target);
    Vector2 text_pos = { 0.0f, 0.0f };
    if (OMG_ISNOTNULL(bg))
        this->raylib->ClearBackground(_OMG_RAYLIB_OMG_COLOR(bg));
    this->raylib->DrawTextEx(font_raylib->font, text->ptr, text_pos, font->size * _RAYLIB_FONT_SIZE_MUL * font->a_scale, font->spacing, _OMG_RAYLIB_OMG_COLOR(fg));
    omg_renderer_raylib_set_target(this, target_cache);
    if (OMG_ISNOTNULL(rect)) {
        rect->x = rect->y = 0.0f;
        rect->w = tex_base->size.w;
        rect->h = tex_base->size.h;
    }
    return tex;
}

bool omg_renderer_raylib_draw_line_ex(OMG_RendererRaylib* this, const OMG_FRect* start_end, float thick, const OMG_Color* col) {
    Vector2 vec1 = { .x = (start_end->x1 + this->so.x) * this->ss.x, .y = (start_end->y1 + this->so.y) * this->ss.y };
    Vector2 vec2 = { .x = (start_end->x2 + this->so.x) * this->ss.x, .y = (start_end->y2 + this->so.y) * this->ss.y };
    this->raylib->DrawLineEx(vec1, vec2, thick * this->ss.w / _RL_THICK_DIV, _OMG_RAYLIB_OMG_COLOR(col));
    return false;
}

bool omg_renderer_raylib_fill_rect_ex(OMG_RendererRaylib* this, const OMG_FRect* rect, float roundness, const OMG_Color* col) {
    if (roundness <= 0.0f)
        return omg_renderer_raylib_fill_rect(this, rect, col);
    RL_Rectangle rec = {
        .x = (rect->x + this->so.x) * this->ss.x, .y = (rect->y + this->so.y) * this->ss.y,
        .width = rect->w * this->ss.x, .height = rect->h * this->ss.y
    };
    this->raylib->DrawRectangleRounded(rec, roundness * this->ss.w, -1, _OMG_RAYLIB_OMG_COLOR(col));
    return false;
}

bool omg_renderer_raylib_draw_rect_ex(OMG_RendererRaylib* this, const OMG_FRect* rect, float roundness, float thick, const OMG_Color* col) {
    RL_Rectangle rec = {
        .x = (rect->x + this->so.x) * this->ss.x, .y = (rect->y + this->so.y) * this->ss.y,
        .width = rect->w * this->ss.x, .height = rect->h * this->ss.y
    };
    if (roundness <= 0.0f) {
        this->raylib->DrawRectangleLinesEx(rec, thick * this->ss.w / _RL_THICK_DIV, _OMG_RAYLIB_OMG_COLOR(col));
        return false;
    }
    this->raylib->DrawRectangleRoundedLines(rec, roundness * this->ss.w, -1, thick * this->ss.w / _RL_THICK_DIV, _OMG_RAYLIB_OMG_COLOR(col));
    return false;
}

bool omg_renderer_raylib_font_render_to(OMG_RendererRaylib* this, const OMG_FPoint* pos, OMG_Font* font, const OMG_String* text, const OMG_Color* bg, const OMG_Color* fg, OMG_FRect* rect) {
    if (OMG_IS_DUMMY_FONT(font) || omg_string_ensure_null((OMG_String*)text))
        return omg_renderer_font_render_to(base, pos, font, text, bg, fg, rect);
    if (OMG_ISNULL(pos)) {
        pos = &OMG_FPOINT_MAKE(0, 0);
    }
    Vector2 tex_size = this->raylib->MeasureTextEx(font_raylib->font, text->ptr, font->size * _RAYLIB_FONT_SIZE_MUL * font->a_scale, font->spacing);
    if (OMG_ISNOTNULL(bg)) {
        RL_Rectangle rec = {
            .x = (pos->x + this->so.x) * this->ss.x, .y = (pos->y + this->so.y) * this->ss.y,
            .width = tex_size.x * this->ss.x, .height = tex_size.y * this->ss.y
        };
        this->raylib->DrawRectangleRec(rec, _OMG_RAYLIB_OMG_COLOR(bg));
    }
    Vector2 text_pos;
    text_pos.x = pos->x;
    text_pos.y = pos->y;
    this->raylib->DrawTextEx(font_raylib->font, text->ptr, text_pos, font->size * _RAYLIB_FONT_SIZE_MUL * font->a_scale, font->spacing, _OMG_RAYLIB_OMG_COLOR(fg));
    if (OMG_ISNOTNULL(rect)) {
        rect->x = rect->y = 0.0f;
        rect->w = tex_size.x;
        rect->h = tex_size.y;
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
    base->draw_line_ex = omg_renderer_raylib_draw_line_ex;
    base->draw_rect = omg_renderer_raylib_draw_rect;
    base->fill_rect = omg_renderer_raylib_fill_rect;
    base->fill_rect_ex = omg_renderer_raylib_fill_rect_ex;
    base->draw_rect_ex = omg_renderer_raylib_draw_rect_ex;
    base->draw_ellipse = omg_renderer_raylib_draw_ellipse;
    base->fill_ellipse = omg_renderer_raylib_fill_ellipse;
    base->draw_circle = omg_renderer_raylib_draw_circle;
    base->fill_circle = omg_renderer_raylib_fill_circle;
    base->tex_set_scale_mode = omg_renderer_raylib_tex_set_scale_mode;
    base->tex_from_surf = omg_renderer_raylib_tex_from_surf;
    base->tex_create = omg_renderer_raylib_tex_create;
    base->tex_destroy = omg_renderer_raylib_tex_destroy;
    base->copy = omg_renderer_raylib_copy;
    base->copy_ex = omg_renderer_raylib_copy_ex;
    base->tex_set_color_mod = omg_renderer_raylib_tex_set_color_mod;
    base->set_blend_mode = omg_renderer_raylib_set_blend_mode;
    base->font_render = omg_renderer_raylib_font_render;
    base->font_render_to = omg_renderer_raylib_font_render_to;
    base->set_clip_rect = omg_renderer_raylib_set_clip_rect;
    OMG_END_POINTER_CAST();
    this->blend_cache = -1;
    this->ss.x = this->ss.y = this->ss.w = 1.0f;
    this->so.x = this->so.y = 0.0f;
    base->soft_offset = true;
    base->soft_scale = true;
    base->type = OMG_REN_TYPE_RAYLIB;
    base->inited = true;
    omg_renderer_raylib_update_scale(this);
    _OMG_LOG_INFO(omg_base, "Raylib renderer created successfuly with opengl driver");
    return false;
}
#endif
