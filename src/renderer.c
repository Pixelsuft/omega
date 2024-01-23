#include <omega/renderer.h>
#include <omega/window.h>
#include <omega/omega.h>
#define win_base ((OMG_Window*)this->win)
#define omg_base ((OMG_Omega*)this->omg)

bool omg_renderer_destroy(OMG_Renderer* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_renderer_clear(OMG_Renderer* this, const OMG_Color* col) {
    OMG_UNUSED(this, col);
    return false;
}

bool omg_renderer_begin(OMG_Renderer* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_renderer_flip(OMG_Renderer* this) {
    OMG_UNUSED(this);
    return false;
}

void omg_on_update_window_size(OMG_Renderer* this) {
    OMG_UNUSED(this);
}

int omg_renderer_get_supported_drivers(OMG_Renderer* this) {
    OMG_UNUSED(this);
    return OMG_REN_DRIVER_SOFTWARE;
}

void omg_renderer_fill_on_create(OMG_Renderer* this) {
    this->driver = OMG_REN_DRIVER_AUTO;
    this->was_allocated = false;
    this->init = omg_renderer_init;
    this->get_supported_drivers = omg_renderer_get_supported_drivers;
}

bool omg_renderer_set_vsync(OMG_Renderer* this, bool enabled) {
    win_base->vsync = enabled;
    return false;
}

bool omg_renderer_set_scale(OMG_Renderer* this, const OMG_FPoint* offset, const OMG_FPoint* scale) {
    if (OMG_ISNOTNULL(offset)) {
        this->offset.x = offset->x;
        this->offset.y = offset->y;
    }
    if (OMG_ISNOTNULL(scale)) {
        this->scale.x = scale->x;
        this->scale.y = scale->y;
        this->a_scale = (this->scale.x + this->scale.y) / 2.0f;
    }
    return false;
}

OMG_Texture* omg_renderer_tex_from_surf(OMG_Renderer* this, OMG_Texture* tex, OMG_Surface* surf, bool destroy_surf) {
    OMG_UNUSED(tex, surf, destroy_surf);
    return omg_renderer_dummy_tex_create(this);
}

OMG_Texture* omg_renderer_tex_create(OMG_Renderer* this, OMG_Texture* tex, const OMG_FPoint* size, int access, bool has_alpha) {
    OMG_UNUSED(tex, size, access, has_alpha);
    return omg_renderer_dummy_tex_create(this);
}

OMG_Texture* omg_renderer_dummy_tex_create(OMG_Renderer* this) {
    OMG_UNUSED(this);
#if OMG_ALLOW_DUMMY_TEX
    static OMG_Texture tex;
    tex.has_alpha = false;
    tex.size.w = tex.size.h = 0.0f;
    return &tex;
#else
    return NULL;
#endif
}

bool omg_renderer_tex_destroy(OMG_Renderer* this, OMG_Texture* tex) {
    OMG_UNUSED(this, tex);
    return false;
}

bool omg_renderer_set_target(OMG_Renderer* this, OMG_Texture* tex) {
    this->target = tex;
    return false;
}

bool omg_renderer_draw_point(OMG_Renderer* this, const OMG_FPoint* pos, const OMG_Color* col) {
    OMG_UNUSED(this, pos, col);
    return false;
}

bool omg_renderer_draw_fill_rect(OMG_Renderer* this, const OMG_FRect* rect, const OMG_Color* col) {
    OMG_UNUSED(this, rect, col);
    return false;
}

bool omg_renderer_draw_circle(OMG_Renderer* this, const OMG_FPoint* pos, float rad, const OMG_Color* col) {
    OMG_UNUSED(this, pos, rad, col);
    return false;
}

bool omg_renderer_copy(OMG_Renderer* this, OMG_Texture* tex, const OMG_FPoint* pos) {
    OMG_UNUSED(this, tex, pos);
    return false;
}

bool omg_renderer_copy_ex(OMG_Renderer* this, OMG_Texture* tex, const OMG_FRect* src, const OMG_FRect* dst, const OMG_FPoint* origin, double rot) {
    OMG_UNUSED(src, origin, rot);
    if (OMG_ISNULL(dst))
        return this->copy(this, tex, NULL);
    return this->copy(this, tex, (const OMG_FPoint*)dst);
}

bool omg_renderer_tex_set_scale_mode(OMG_Renderer* this, OMG_Texture* tex, int scale_mode) {
    OMG_UNUSED(this, tex, scale_mode);
    return false;
}

bool omg_renderer_tex_set_color_mod(OMG_Renderer* this, OMG_Texture* tex, const OMG_Color* col) {
    OMG_UNUSED(this, tex, col);
    return false;
}

bool omg_renderer_tex_set_blend_mode(OMG_Renderer* this, OMG_Texture* tex, int blend_mode) {
    OMG_UNUSED(this, tex, blend_mode);
    return false;
}

bool omg_renderer_set_blend_mode(OMG_Renderer* this, int blend_mode) {
    OMG_UNUSED(this, blend_mode);
    return false;
}

OMG_Texture* omg_rednerer_font_render(OMG_Renderer* this, OMG_Texture* tex, OMG_Font* font, const OMG_String* text, const OMG_Color* bg, const OMG_Color* fg, const OMG_FRect* rect) {
    OMG_UNUSED(tex, font, bg, fg);
    if (OMG_ISNOTNULL(rect)) {
        rect->x = rect->y = rect->w = rect->h = 0.0f;
    }
    return omg_renderer_dummy_tex_create(this);
}

bool omg_renderer_init(OMG_Renderer* this) {
    this->type = OMG_REN_TYPE_NONE;
    this->inited = false;
    this->omg = win_base->omg;
    this->aa = false;
    this->soft_scale = false;
    this->soft_offset = false;
    this->auto_blend = true;
    this->target = NULL;
    this->fix_auto_tex_blend = true;
    this->tex_default_scale_mode = OMG_SCALE_MODE_AUTO;
    this->offset.x = this->offset.y = 0.0f;
    this->scale.x = this->scale.y = this->a_scale = 1.0f;
    this->dpi_scale.x = this->dpi_scale.y = 1.0f;
    this->color.r = this->color.g = this->color.b = (omg_color_t)0;
    this->color.a = (omg_color_t)255;
    this->scale.x = win_base->scale.x;
    this->scale.y = win_base->scale.y;
    this->size.w = win_base->size.w;
    this->size.h = win_base->size.h;
    this->_on_update_window_size = omg_on_update_window_size;
    this->set_vsync = omg_renderer_set_vsync;
    this->destroy = omg_renderer_destroy;
    this->clear = omg_renderer_clear;
    this->begin = omg_renderer_begin;
    this->flip = omg_renderer_flip;
    this->set_scale = omg_renderer_set_scale;
    this->set_target = omg_renderer_set_target;
    this->draw_point = omg_renderer_draw_point;
    this->draw_line = omg_renderer_draw_fill_rect;
    this->draw_rect = omg_renderer_draw_fill_rect;
    this->fill_rect = omg_renderer_draw_fill_rect;
    this->draw_circle = omg_renderer_draw_circle;
    this->fill_circle = omg_renderer_draw_circle;
    this->tex_from_surf = omg_renderer_tex_from_surf;
    this->tex_create = omg_renderer_tex_create;
    this->tex_destroy = omg_renderer_tex_destroy;
    this->copy = omg_renderer_copy;
    this->copy_ex = omg_renderer_copy_ex;
    this->tex_set_scale_mode = omg_renderer_tex_set_scale_mode;
    this->tex_set_color_mod = omg_renderer_tex_set_color_mod;
    this->tex_set_blend_mode = omg_renderer_tex_set_blend_mode;
    this->set_blend_mode = omg_renderer_set_blend_mode;
    return false;
}
