#include <omega/renderer_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/window.h>
#include <omega/omega.h>
#include <omega/surface_sdl2.h>
#include <omega/texture_sdl2.h>
#include <omega/api_sdl2_gfx.h>
#define base ((OMG_Renderer*)this)
#define tex_base ((OMG_Texture*)tex)
#define win_base ((OMG_Window*)base->win)
#define omg_base ((OMG_Omega*)base->omg)
#define OMG_TEX_ACCESS_TO_SDL2(access) ((access) == OMG_TEXTURE_ACCESS_STREAMING) ? SDL_TEXTUREACCESS_STREAMING : (((access) == OMG_TEXTURE_ACCESS_TARGET) ? SDL_TEXTUREACCESS_TARGET : SDL_TEXTUREACCESS_STATIC)
#define SDL2_TEX_ACCESS_TO_OMG(access) ((access) == SDL_TEXTUREACCESS_STREAMING) ? OMG_TEXTURE_ACCESS_STREAMING : (((access) == SDL_TEXTUREACCESS_TARGET) ? OMG_TEXTURE_ACCESS_TARGET : OMG_TEXTURE_ACCESS_STATIC)
#define MAKE_SDL2_RECT(rect) { rect->x + base->offset.x, rect->y + base->offset.y, rect->w, rect->h }
#define APPLY_SDL2_DRAW(res, col) \
    uint8_t _r_color = (uint8_t)(col->r * (omg_color_t)255 / OMG_MAX_COLOR); \
    uint8_t _g_color = (uint8_t)(col->g * (omg_color_t)255 / OMG_MAX_COLOR); \
    uint8_t _b_color = (uint8_t)(col->b * (omg_color_t)255 / OMG_MAX_COLOR); \
    uint8_t _a_color = (uint8_t)(col->a * (omg_color_t)255 / OMG_MAX_COLOR); \
    if (this->sdl2->SDL_SetRenderDrawColor( \
        this->ren, \
        _r_color, \
        _g_color, \
        _b_color, \
        _a_color \
    ) < 0) { \
        res = true; \
        _OMG_SDL2_DRAW_COLOR_WARN(); \
    } \
    if ( \
        base->auto_blend && \
        (this->sdl2->SDL_SetRenderDrawBlendMode(this->ren, ((_a_color == 255) && !base->aa) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND) < 0) \
    ) { \
        res = true; \
        _OMG_SDL2_DRAW_BLEND_WARN(); \
    }
#define SDL2_SCALE_OFF(res) do { \
    if ((base->scale.x != 1.0f) || (base->scale.y != 1.0f)) { \
        if (this->sdl2->SDL_RenderSetScale(this->ren, 1.0f, 1.0f) < 0) { \
            res = true; \
            _OMG_SDL2_SCALE_WARN(); \
        } \
    } \
} while (0)
#define SDL2_SCALE_ON(res) do { \
    if ((base->scale.x != 1.0f) || (base->scale.y != 1.0f)) { \
        if (this->sdl2->SDL_RenderSetScale(this->ren, base->scale.x, base->scale.y) < 0) { \
            res = true; \
            _OMG_SDL2_SCALE_WARN(); \
        } \
    } \
} while (0)
#define SDL2_TEX_APPLY_SCALE_MODE(tex, mode) do { \
    if ((mode != OMG_SCALE_MODE_AUTO) && this->sdl2->SDL_SetTextureScaleMode(tex, ( \
        (mode == OMG_SCALE_MODE_LINEAR) ? SDL_ScaleModeLinear : SDL_ScaleModeNearest \
    )) < 0) \
        _OMG_LOG_WARN(omg_base, "Failed to set texture scale mode (", this->sdl2->SDL_GetError(), ")"); \
} while (0)

void omg_renderer_sdl2_update_scale(OMG_RendererSdl2* this) {
    // if (!omg_base->support_highdpi)
    //     return;
    int win_w, win_h, ren_w, ren_h;
    this->sdl2->SDL_GetWindowSize(this->win, &win_w, &win_h);
    if (this->sdl2->SDL_GetRendererOutputSize(this->ren, &ren_w, &ren_h) < 0) {
        _OMG_LOG_INFO(omg_base, "Failed to get renderer size (", this->sdl2->SDL_GetError(), ")");
        return;
    }
    base->size.w = (float)ren_w;
    base->size.h = (float)ren_h;
    base->dpi_scale.x = base->size.w / (float)win_w;
    base->dpi_scale.y = base->size.h / (float)win_h;
}

bool omg_renderer_sdl2_destroy(OMG_RendererSdl2* this) {
    if (base->inited) {
        omg_sdl2_gfx_set_handle(NULL, NULL);
        this->sdl2->SDL_DestroyRenderer(this->ren);
        this->ren = NULL;
        base->inited = false;
    }
    return false;
}

int omg_renderer_sdl2_driver_from_name(OMG_RendererSdl2* this, const char* name) {
    if (OMG_ISNULL(name))
        return OMG_REN_DRIVER_UNKNOWN;
    if (!omg_base->std->strcmp(name, "direct3d"))
        return OMG_REN_DRIVER_D3D9;
    if (!omg_base->std->strcmp(name, "direct3d11"))
        return OMG_REN_DRIVER_D3D11;
    if (!omg_base->std->strcmp(name, "direct3d12"))
        return OMG_REN_DRIVER_D3D12;
    if (!omg_base->std->strcmp(name, "opengl"))
        return OMG_REN_DRIVER_OPENGL;
    if (!omg_base->std->strcmp(name, "opengles"))
        return OMG_REN_DRIVER_OPENGLES;
    if (!omg_base->std->strcmp(name, "opengles2"))
        return OMG_REN_DRIVER_OPENGLES;
    if (!omg_base->std->strcmp(name, "software"))
        return OMG_REN_DRIVER_SOFTWARE;
    return OMG_REN_DRIVER_UNKNOWN;
}

int omg_renderer_sdl2_get_renderer_id(OMG_RendererSdl2* this, int driver) {
    for (int i = 0; i < this->id_cache[9]; i++) {
        if (this->id_cache[i] == driver)
            return i;
    }
    return -1;
}

int omg_renderer_sdl2_get_supported_drivers(OMG_RendererSdl2* this) {
    if (this->id_cache[0] >= 0)
        return this->id_cache[8];
    int res = OMG_REN_DRIVER_NONE;
    int num_drivers = this->sdl2->SDL_GetNumRenderDrivers();
    if (num_drivers < 0) {
        _OMG_LOG_ERROR(omg_base, "Failed to get number of render drivers (", this->sdl2->SDL_GetError(), ")");
        return res;
    }
    if (num_drivers > 8)
        num_drivers = 8;
    this->id_cache[9] = num_drivers;
    SDL_RendererInfo info;
    for (int i = 0; i < num_drivers; i++) {
        if (this->sdl2->SDL_GetRenderDriverInfo(i, &info) < 0) {
            this->id_cache[i] = OMG_REN_DRIVER_NONE;
            continue;
        }
        this->id_cache[i] = omg_renderer_sdl2_driver_from_name(this, info.name);
        res |= this->id_cache[i];
    }
    this->id_cache[8] = res;
    return res;
}

bool omg_renderer_sdl2_clear(OMG_RendererSdl2* this, const OMG_Color* col) {
    if (OMG_ISNULL(col))
        col = &base->color;
    bool res = false;
    if (this->sdl2->SDL_SetRenderDrawColor(
        this->ren,
        (uint8_t)(col->r * (omg_color_t)255 / OMG_MAX_COLOR),
        (uint8_t)(col->g * (omg_color_t)255 / OMG_MAX_COLOR), 
        (uint8_t)(col->b * (omg_color_t)255 / OMG_MAX_COLOR),
        (uint8_t)(col->a * (omg_color_t)255 / OMG_MAX_COLOR)
    ) < 0) {
        res = true;
        _OMG_SDL2_DRAW_COLOR_WARN();
    }
    if (this->sdl2->SDL_RenderClear(this->ren) < 0) {
        res = true;
        _OMG_LOG_WARN(omg_base, "Failed to clear renderer (", this->sdl2->SDL_GetError(), ")");
    }
    return res;
}

bool omg_renderer_sdl2_set_scale(OMG_RendererSdl2* this, const OMG_FPoint* offset, const OMG_FPoint* scale) {
    omg_renderer_set_scale(base, offset, scale);
    if (this->sdl2->SDL_RenderSetScale(this->ren, base->scale.x, base->scale.y) < 0) {
        _OMG_SDL2_SCALE_WARN();
        return true;
    }
    return false;
}

bool omg_renderer_sdl2_set_target(OMG_RendererSdl2* this, OMG_TextureSdl2* tex) {
    if (this->sdl2->SDL_SetRenderTarget(this->ren, OMG_ISNULL(tex) ? NULL : tex->tex) < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to set render target (", this->sdl2->SDL_GetError(), ")");
        return true;
    }
    base->target = tex_base;
    omg_renderer_sdl2_set_scale(this, NULL, NULL);
    return false;
}

bool omg_renderer_sdl2_draw_point(OMG_RendererSdl2* this, const OMG_FPoint* pos, const OMG_Color* col) {
    if (OMG_ISNULL(col))
        col = &base->color;
    bool res = false;
    APPLY_SDL2_DRAW(res, col);
    if (this->sdl2->SDL_RenderDrawPointF(this->ren, pos->x + base->offset.x, pos->y + base->offset.y) < 0) {
        res = true;
        _OMG_LOG_WARN(omg_base, "Failed to draw point (", this->sdl2->SDL_GetError(), ")");
    }
    return res;
}

bool omg_renderer_sdl2_draw_rect(OMG_RendererSdl2* this, const OMG_FRect* rect, const OMG_Color* col) {
    if (OMG_ISNULL(col))
        col = &base->color;
    bool res = false;
    APPLY_SDL2_DRAW(res, col);
    SDL_FRect sdl_rect = MAKE_SDL2_RECT(rect);
    if (this->sdl2->SDL_RenderDrawRectF(this->ren, &sdl_rect) < 0) {
        res = true;
        _OMG_LOG_WARN(omg_base, "Failed to draw rect (", this->sdl2->SDL_GetError(), ")");
    }
    return res;
}

bool omg_renderer_sdl2_fill_rect(OMG_RendererSdl2* this, const OMG_FRect* rect, const OMG_Color* col) {
    if (OMG_ISNULL(col))
        col = &base->color;
    bool res = false;
    APPLY_SDL2_DRAW(res, col);
    SDL_FRect sdl_rect = MAKE_SDL2_RECT(rect);
    if (this->sdl2->SDL_RenderFillRectF(this->ren, &sdl_rect) < 0) {
        res = true;
        _OMG_LOG_WARN(omg_base, "Failed to fill rect (", this->sdl2->SDL_GetError(), ")");
    }
    return res;
}

bool omg_renderer_sdl2_draw_line(OMG_RendererSdl2* this, const OMG_FRect* start_end, const OMG_Color* col) {
    if (OMG_ISNULL(col))
        col = &base->color;
    bool res = false;
    APPLY_SDL2_DRAW(res, col);
    if (base->aa) {
        /*aaLineRGBA(
            this->ren,
            (int16_t)(start_end->x1 + base->offset.x),
            (int16_t)(start_end->y1 + base->offset.y),
            (int16_t)(start_end->x2 + base->offset.x),
            (int16_t)(start_end->y2 + base->offset.y),
            _r_color, _g_color, _b_color, _a_color
        );*/
        SDL2_SCALE_OFF(res);
        aaLineRGBA(
            this->ren,
            (int16_t)((start_end->x1 + base->offset.x) * base->scale.x),
            (int16_t)((start_end->y1 + base->offset.y) * base->scale.y),
            (int16_t)((start_end->x2 + base->offset.x) * base->scale.x),
            (int16_t)((start_end->y2 + base->offset.y) * base->scale.y),
            _r_color, _g_color, _b_color, _a_color
        );
        SDL2_SCALE_ON(res);
    }
    else if (this->sdl2->SDL_RenderDrawLineF(this->ren, start_end->x1 + base->offset.x, start_end->y1 + base->offset.y, start_end->x2 + base->offset.x, start_end->y2 + base->offset.y) < 0) {
        res = true;
        _OMG_LOG_WARN(omg_base, "Failed to draw line (", this->sdl2->SDL_GetError(), ")");
    }
    return res;
}

bool omg_renderer_sdl2_draw_circle(OMG_RendererSdl2* this, const OMG_FPoint* pos, float rad, const OMG_Color* col) {
    bool res = false;
    APPLY_SDL2_DRAW(res, col);
    SDL2_SCALE_OFF(res);
    if (base->aa) {
        aaEllipseRGBA(
            this->ren,
            (int16_t)((pos->x + base->offset.x) * base->scale.x),
            (int16_t)((pos->y + base->offset.y) * base->scale.y),
            (int16_t)(rad * base->scale.x),
            (int16_t)(rad * base->scale.y),
            _r_color, _g_color, _b_color, _a_color
        );
    }
    else {
        drawEllipse(
            this->ren,
            (int16_t)((pos->x + base->offset.x) * base->scale.x),
            (int16_t)((pos->y + base->offset.y) * base->scale.y),
            (int16_t)(rad * base->scale.x),
            (int16_t)(rad * base->scale.y)
        );
    }
    SDL2_SCALE_ON(res);
    return res;
}

bool omg_renderer_sdl2_fill_circle(OMG_RendererSdl2* this, const OMG_FPoint* pos, float rad, const OMG_Color* col) {
    bool res = false;
    APPLY_SDL2_DRAW(res, col);
    SDL2_SCALE_OFF(res);
    if (base->aa) {
        aaFilledEllipseRGBA(
            this->ren,
            (int16_t)((pos->x + base->offset.x) * base->scale.x),
            (int16_t)((pos->y + base->offset.y) * base->scale.y),
            (int16_t)(rad * base->scale.x),
            (int16_t)(rad * base->scale.y),
            _r_color, _g_color, _b_color, _a_color
        );
    }
    else {
        filledEllipse(
            this->ren,
            (int16_t)((pos->x + base->offset.x) * base->scale.x),
            (int16_t)((pos->y + base->offset.y) * base->scale.y),
            (int16_t)(rad * base->scale.x),
            (int16_t)(rad * base->scale.y)
        );
    }
    SDL2_SCALE_ON(res);
    return res;
}

bool omg_renderer_sdl2_flip(OMG_RendererSdl2* this) {
    if (OMG_ISNOTNULL(base->target))
        omg_renderer_sdl2_set_target(this, NULL);
    this->sdl2->SDL_RenderPresent(this->ren);
    return false;
}

OMG_TextureSdl2* omg_renderer_sdl2_tex_from_surf(OMG_RendererSdl2* this, OMG_Surface* surf, bool destroy_surf) {
    if (OMG_ISNULL(surf))
        return (OMG_TextureSdl2*)omg_renderer_tex_from_surf(base, (OMG_Surface*)surf, destroy_surf);
    OMG_TextureSdl2* tex = OMG_MALLOC(omg_base->mem, sizeof(OMG_TextureSdl2));
    if (OMG_ISNULL(tex))
        return (OMG_TextureSdl2*)omg_renderer_tex_from_surf(base, (OMG_Surface*)surf, destroy_surf);
    tex->temp_surf = NULL;
    if (win_base->type == OMG_WIN_TYPE_SDL2) {
        tex->tex = this->sdl2->SDL_CreateTextureFromSurface(this->ren, ((OMG_SurfaceSdl2*)surf)->surf);
        if (OMG_ISNULL(tex->tex)) {
            OMG_FREE(omg_base->mem, tex);
            _OMG_LOG_ERROR(omg_base, "Failed to create SDL2 texture from surface (", this->sdl2->SDL_GetError(), ")");
            return (OMG_TextureSdl2*)omg_renderer_tex_from_surf(base, (OMG_Surface*)surf, destroy_surf);
        }
        tex_base->has_alpha = surf->has_alpha;
        tex_base->size.w = surf->size.w;
        tex_base->size.h = surf->size.h;
        if (destroy_surf)
            omg_base->winmgr->surf_destroy(omg_base->winmgr, surf);
        SDL2_TEX_APPLY_SCALE_MODE(tex->tex, base->tex_default_scale_mode);
        return tex;
    }
#if OMG_SUPPORT_WIN
    else if ((win_base->type == OMG_WIN_TYPE_WIN) && OMG_ISNOTNULL(surf->data)) {
        tex->temp_surf = this->sdl2->SDL_CreateRGBSurfaceWithFormatFrom(
            surf->data,
            (int)surf->size.w, (int)surf->size.h,
            omg_base->winmgr->surf_depth, (int)surf->size.w * 4,
            SDL_PIXELFORMAT_ABGR8888
        );
        if (OMG_ISNULL(tex->temp_surf)) {
            OMG_FREE(omg_base->mem, tex);
            return (OMG_TextureSdl2*)omg_renderer_tex_from_surf(base, (OMG_Surface*)surf, destroy_surf);
        }
        tex->tex = this->sdl2->SDL_CreateTextureFromSurface(this->ren, tex->temp_surf);
        // WTF
        // this->sdl2->SDL_FreeSurface(tex->temp_surf);
        if (OMG_ISNULL(tex->tex)) {
            this->sdl2->SDL_FreeSurface(tex->temp_surf);
            OMG_FREE(omg_base->mem, tex);
            _OMG_LOG_ERROR(omg_base, "Failed to create SDL2 texture from Win32 surface (", this->sdl2->SDL_GetError(), ")");
            return (OMG_TextureSdl2*)omg_renderer_tex_from_surf(base, (OMG_Surface*)surf, destroy_surf);
        }
        tex_base->has_alpha = surf->has_alpha;
        tex_base->size.w = (float)tex->temp_surf->w;
        tex_base->size.h = (float)tex->temp_surf->h;
        if (destroy_surf)
            omg_base->winmgr->surf_destroy(omg_base->winmgr, surf);
        SDL2_TEX_APPLY_SCALE_MODE(tex->tex, base->tex_default_scale_mode);
        return tex;
    }
#endif
    else {
        OMG_FREE(omg_base->mem, tex);
        return (OMG_TextureSdl2*)omg_renderer_tex_from_surf(base, (OMG_Surface*)surf, destroy_surf);
    }
}

OMG_TextureSdl2* omg_renderer_sdl2_tex_create(OMG_RendererSdl2* this, const OMG_FPoint* size, int access, bool has_alpha) {
    OMG_TextureSdl2* tex = OMG_MALLOC(omg_base->mem, sizeof(OMG_TextureSdl2));
    if (OMG_ISNULL(tex))
        return (OMG_TextureSdl2*)omg_renderer_tex_create(base, size, access, has_alpha);
    tex->temp_surf = NULL;
    tex->tex = this->sdl2->SDL_CreateTexture(
        this->ren,
        has_alpha ? SDL_PIXELFORMAT_RGBA8888 : SDL_PIXELFORMAT_RGB888,
        OMG_TEX_ACCESS_TO_SDL2(access),
        (int)size->w, (int)size->h
    );
    if (OMG_ISNULL(tex->tex)) {
        OMG_FREE(omg_base->mem, tex);
        _OMG_LOG_ERROR(omg_base, "Failed to create SDL2 texture (", this->sdl2->SDL_GetError(), ")");
        return (OMG_TextureSdl2*)omg_renderer_tex_create(base, size, access, has_alpha);
    }
    int qw, qh;
    if (this->sdl2->SDL_QueryTexture(tex->tex, NULL, NULL, &qw, &qh) < 0) {
        tex_base->size.w = tex_base->size.h = 0.0f;
        _OMG_LOG_WARN(omg_base, "Failed to query SDL2 texture (", this->sdl2->SDL_GetError(), ")");
    }
    else {
        tex_base->size.w = (float)qw;
        tex_base->size.h = (float)qh;
    }
    tex_base->has_alpha = true;
    if (this->sdl2->SDL_SetTextureBlendMode(tex->tex, has_alpha ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE) < 0)
        _OMG_LOG_WARN(omg_base, "Failed to set texture blend mode (", this->sdl2->SDL_GetError(), ")");
    SDL2_TEX_APPLY_SCALE_MODE(tex->tex, base->tex_default_scale_mode);
    return tex;
}

bool omg_renderer_sdl2_tex_destroy(OMG_RendererSdl2* this, OMG_TextureSdl2* tex) {
    if (OMG_ISNULL(tex)) {
        _OMG_NULL_TEXTURE_WARN();
        return true;
    }
    if (OMG_ISNULL(tex->tex)) {
        _OMG_LOG_WARN(omg_base, "Attempted to free SDL2 null texture");
        return true;
    }
    this->sdl2->SDL_DestroyTexture(tex->tex);
    tex->tex = NULL;
    if (OMG_ISNOTNULL(tex->temp_surf)) {
        this->sdl2->SDL_FreeSurface(tex->temp_surf);
        tex->temp_surf = NULL;
    }
    OMG_FREE(omg_base->mem, tex);
    return false;
}

bool omg_renderer_sdl2_copy(OMG_RendererSdl2* this, OMG_TextureSdl2* tex, const OMG_FPoint* pos) {
    SDL_FRect dst_rect;
    dst_rect.x = base->offset.x;
    dst_rect.y = base->offset.y;
    if (OMG_ISNOTNULL(pos)) {
        dst_rect.x += pos->x;
        dst_rect.y += pos->y;
    }
    dst_rect.w = tex_base->size.w;
    dst_rect.h = tex_base->size.h;
    if (this->sdl2->SDL_RenderCopyF(this->ren, tex->tex, NULL, &dst_rect) < 0) {
        _OMG_SDL2_COPY_WARN();
        return true;
    }
    return false;
}

bool omg_renderer_sdl2_copy_ex(OMG_RendererSdl2* this, OMG_TextureSdl2* tex, const OMG_FRect* src, const OMG_FRect* dst, const OMG_FPoint* origin, const double rot) {
    SDL_FRect dst_rect;
    SDL_Rect src_rect;
    SDL_RendererFlip flip = 0;
    if (OMG_ISNULL(dst)) {
        dst_rect.x = base->offset.x;
        dst_rect.y = base->offset.y;
        dst_rect.w = tex_base->size.w;
        dst_rect.h = tex_base->size.h;
    }
    else {
        dst_rect.x = dst->x + base->offset.x;
        dst_rect.y = dst->y + base->offset.y;
        dst_rect.w = dst->w;
        dst_rect.h = dst->h;
        if (dst_rect.w == 0.0f)
            dst_rect.w = tex_base->size.w;
        else if (dst_rect.w < 0.0f) {
            dst_rect.w = -dst_rect.w;
            flip |= SDL_FLIP_HORIZONTAL;
        }
        if (dst_rect.h == 0.0f)
            dst_rect.h = tex_base->size.h;
        else if (dst_rect.h < 0.0f) {
            dst_rect.h = -dst_rect.h;
            flip |= SDL_FLIP_VERTICAL;
        }
    }
    if (OMG_ISNOTNULL(src)) {
        src_rect.x = (int)src->x;
        src_rect.y = (int)src->y;
        src_rect.w = (int)src->w;
        src_rect.h = (int)src->h;
    }
    if (this->sdl2->SDL_RenderCopyExF(
        this->ren, tex->tex, OMG_ISNULL(src) ? NULL : &src_rect, &dst_rect, rot, (const SDL_FPoint*)origin, flip
    ) < 0) {
        _OMG_SDL2_COPY_WARN();
        return true;
    }
    return false;
}

bool omg_renderer_sdl2_tex_set_scale_mode(OMG_RendererSdl2* this, OMG_TextureSdl2* tex, int scale_mode) {
    if (this->sdl2->SDL_SetTextureScaleMode(tex->tex, (
        (scale_mode == OMG_SCALE_MODE_LINEAR) ? SDL_ScaleModeLinear : (scale_mode == OMG_SCALE_MODE_NEAREST ? SDL_ScaleModeNearest : SDL_ScaleModeBest)
    )) < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to set texture blend mode (", this->sdl2->SDL_GetError(), ")");
        return true;
    }
    return false;
}

bool omg_renderer_sdl2_tex_set_color_mod(OMG_RendererSdl2* this, OMG_TextureSdl2* tex, const OMG_Color* col) {
    bool res = false;
    if (this->sdl2->SDL_SetTextureColorMod(
        tex->tex,
        (uint8_t)(col->r * (omg_color_t)255 / OMG_MAX_COLOR),
        (uint8_t)(col->g * (omg_color_t)255 / OMG_MAX_COLOR),
        (uint8_t)(col->b * (omg_color_t)255 / OMG_MAX_COLOR)
    ) < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to set texture color mod (", this->sdl2->SDL_GetError(), ")");
        res = true;
    }
    if (this->sdl2->SDL_SetTextureAlphaMod(tex->tex, (uint8_t)(col->a * (omg_color_t)255 / OMG_MAX_COLOR)) < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to set texture alpha mod (", this->sdl2->SDL_GetError(), ")");
        res = true;
    }
    // I'm lazy to check blend mode here, so user must set it manually for non-alpha image
    return res;
}

bool omg_renderer_sdl2_init(OMG_RendererSdl2* this) {
    OMG_BEGIN_POINTER_CAST();
    omg_renderer_init(this);
    omg_renderer_sdl2_get_supported_drivers(this);
    base->_on_update_window_size = omg_renderer_sdl2_update_scale;
    base->destroy = omg_renderer_sdl2_destroy;
    base->clear = omg_renderer_sdl2_clear;
    base->flip = omg_renderer_sdl2_flip;
    base->set_scale = omg_renderer_sdl2_set_scale;
    base->set_target = omg_renderer_sdl2_set_target;
    base->draw_point = omg_renderer_sdl2_draw_point;
    base->draw_line = omg_renderer_sdl2_draw_line;
    base->draw_rect = omg_renderer_sdl2_draw_rect;
    base->fill_rect = omg_renderer_sdl2_fill_rect;
    base->draw_circle = omg_renderer_sdl2_draw_circle;
    base->fill_circle = omg_renderer_sdl2_fill_circle;
    base->tex_from_surf = omg_renderer_sdl2_tex_from_surf;
    base->tex_create = omg_renderer_sdl2_tex_create;
    base->tex_destroy = omg_renderer_sdl2_tex_destroy;
    base->copy = omg_renderer_sdl2_copy;
    base->copy_ex = omg_renderer_sdl2_copy_ex;
    base->tex_set_scale_mode = omg_renderer_sdl2_tex_set_scale_mode;
    base->tex_set_color_mod = omg_renderer_sdl2_tex_set_color_mod;
    OMG_END_POINTER_CAST();
    base->type = OMG_REN_TYPE_SDL2;
    int sdl2_driver;
    if (base->driver == OMG_REN_DRIVER_AUTO) {
        sdl2_driver = -1;
        if ((this->id_cache[8] & OMG_REN_DRIVER_D3D12) && (this->win_build_num >= 10240))
            sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_D3D12);
        else if ((this->id_cache[8] & OMG_REN_DRIVER_D3D11) && (this->win_build_num >= 7600))
            sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_D3D11);
        else if (this->id_cache[8] & OMG_REN_DRIVER_D3D9)
            sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_D3D9);
        else if (!OMG_IS_ANDROID && (this->id_cache[8] & OMG_REN_DRIVER_OPENGL))
            sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_OPENGL);
        else if (this->id_cache[8] & OMG_REN_DRIVER_OPENGLES2)
            sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_OPENGLES2);
        else if (this->id_cache[8] & OMG_REN_DRIVER_OPENGLES)
            sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_OPENGLES);
        else if (this->id_cache[8] & OMG_REN_DRIVER_SOFTWARE)
            sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_SOFTWARE);
    }
    else
        sdl2_driver = omg_renderer_sdl2_get_renderer_id(this, base->driver);
    this->ren = this->sdl2->SDL_CreateRenderer(
        this->win,
        sdl2_driver,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | (win_base->vsync ? SDL_RENDERER_PRESENTVSYNC : 0)
    );
    /*if (OMG_ISNULL(this->ren) && (sdl2_driver == omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_D3D12))) {
        base->driver = OMG_REN_DRIVER_D3D11;
        return omg_renderer_sdl2_init(this);
    }
    else if (OMG_ISNULL(this->ren) && (sdl2_driver == omg_renderer_sdl2_get_renderer_id(this, OMG_REN_DRIVER_D3D11))) {
        base->driver = OMG_REN_DRIVER_D3D9;
        return omg_renderer_sdl2_init(this);
    }*/
    if (OMG_ISNULL(this->ren)) {
        _OMG_LOG_ERROR(omg_base, "Failed to create SDL2 renderer (", this->sdl2->SDL_GetError(), ")");
        return true;
    }
    SDL_RendererInfo info;
    if (this->sdl2->SDL_GetRendererInfo(this->ren, &info) < 0)
        _OMG_LOG_WARN(omg_base, "Failed to get renderer info (", this->sdl2->SDL_GetError(), ")");
    else {
        base->driver = omg_renderer_sdl2_driver_from_name(this, info.name);
        _OMG_LOG_INFO(omg_base, "SDL2 renderer created successfuly with ", info.name, " driver");
    }
    omg_sdl2_gfx_set_handle(this->sdl2, omg_std_get_default_handle());
    omg_renderer_sdl2_update_scale(this);
    base->inited = true;
    return false;
}
#endif
