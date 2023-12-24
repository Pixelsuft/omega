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

bool omg_renderer_init(OMG_Renderer* this) {
    this->type = OMG_REN_TYPE_NONE;
    this->inited = false;
    this->omg = win_base->omg;
    this->destroy = omg_renderer_destroy;
    this->clear = omg_renderer_clear;
    this->begin = omg_renderer_begin;
    this->flip = omg_renderer_flip;
    return false;
}
