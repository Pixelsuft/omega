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
    return OMG_REN_DRIVER_NONE;
}

void omg_renderer_fill_on_create(OMG_Renderer* this) {
    this->driver = OMG_REN_DRIVER_AUTO;
    this->was_allocated = false;
    this->init = omg_renderer_init;
    this->get_supported_drivers = omg_renderer_get_supported_drivers;
}


OMG_Texture* omg_renderer_tex_create(OMG_Renderer* this, const OMG_FPoint* size, int access, bool has_alpha) {
    OMG_UNUSED(this, size, access, has_alpha);
    return NULL;
}

bool omg_renderer_tex_destroy(OMG_Renderer* this, OMG_Texture* tex) {
    OMG_UNUSED(this, tex);
    return false;
}

bool omg_renderer_set_target(OMG_Renderer* this, OMG_Texture* tex) {
    this->target = tex;
    return false;
}

bool omg_renderer_init(OMG_Renderer* this) {
    this->type = OMG_REN_TYPE_NONE;
    this->inited = false;
    this->omg = win_base->omg;
    this->aa = false;
    this->target = NULL;
    this->offset.x = this->offset.y = 0.0f;
    this->color.r = this->color.g = this->color.b = (omg_color_t)0;
    this->color.a = (omg_color_t)255;
    this->scale.x = win_base->scale.x;
    this->scale.y = win_base->scale.y;
    this->size.w = win_base->size.w;
    this->size.h = win_base->size.h;
    this->_on_update_window_size = omg_on_update_window_size;
    this->destroy = omg_renderer_destroy;
    this->clear = omg_renderer_clear;
    this->begin = omg_renderer_begin;
    this->flip = omg_renderer_flip;
    this->set_target = omg_renderer_set_target;
    this->tex_create = omg_renderer_tex_create;
    this->tex_destroy = omg_renderer_tex_destroy;
    return false;
}
