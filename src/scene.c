#include <omega/scene.h>

#if OMG_SUPPORT_SCENES
#include <omega/omega.h>
#include <omega/window.h>
#include <omega/renderer.h>

#define ren ((OMG_Renderer*)this->omg_ren)
#define omg_base ((OMG_Omega*)this->omg_omg)


void omg_scenemgr_event_on_update(OMG_EventUpdate* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_paint(OMG_EventPaint* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_expose(OMG_EventExpose* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_size_change(OMG_EventResize* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_loop_stop(OMG_EventLoopStop* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_mouse_wheel(OMG_EventMouseWheel* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_mouse_move(OMG_EventMouseMove* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_mouse_down(OMG_EventMouseButton* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_mouse_up(OMG_EventMouseButton* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_mouse_enter(OMG_EventMouseFocus* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_mouse_leave(OMG_EventMouseFocus* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_key_down(OMG_EventKeyboard* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_key_up(OMG_EventKeyboard* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_text_input(OMG_EventTextInput* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_touch_down(OMG_EventTouch* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_touch_up(OMG_EventTouch* event) {
    OMG_UNUSED(event);
}

void omg_scenemgr_event_on_touch_move(OMG_EventTouch* event) {
    OMG_UNUSED(event);
}

bool omg_scenemgr_init(OMG_SceneMgr* this, void* omg_ren) {
    this->omg_ren = omg_ren;
    this->omg_omg = ren->omg;
    // omg_base->std->memset((void*)((size_t)this->on_update), 0, (size_t)this->on_touch_move - (size_t)this->on_update);
    omg_base->std->memcpy(
        (void*)((size_t)this->on_update),
        (void*)((size_t)omg_base->on_update),
        (size_t)this->on_touch_move - (size_t)this->on_update
    );
    this->on_update = omg_scenemgr_event_on_update;
    this->on_loop_stop = omg_scenemgr_event_on_loop_stop;
    this->on_paint = omg_scenemgr_event_on_paint;
    this->on_expose = omg_scenemgr_event_on_expose;
    this->on_size_change = omg_scenemgr_event_on_size_change;
    this->on_mouse_move = omg_scenemgr_event_on_mouse_move;
    this->on_mouse_down = omg_scenemgr_event_on_mouse_down;
    this->on_mouse_up = omg_scenemgr_event_on_mouse_up;
    this->on_mouse_wheel = omg_scenemgr_event_on_mouse_wheel;
    this->on_mouse_enter = omg_scenemgr_event_on_mouse_enter;
    this->on_mouse_leave = omg_scenemgr_event_on_mouse_leave;
    this->on_key_down = omg_scenemgr_event_on_key_down;
    this->on_key_up = omg_scenemgr_event_on_key_up;
    this->on_text_input = omg_scenemgr_event_on_text_input;
    this->on_touch_down = omg_scenemgr_event_on_touch_down;
    this->on_touch_up = omg_scenemgr_event_on_touch_up;
    this->on_touch_move = omg_scenemgr_event_on_touch_move;
    _OMG_LOG_INFO(omg_base, "Omega scenes successfully inited");
    return false;
}

bool omg_scenemgr_destroy(OMG_SceneMgr* this) {
    omg_base->std->memcpy(
        (void*)((size_t)omg_base->on_update),
        (void*)((size_t)this->on_update),
        (size_t)this->on_touch_move - (size_t)this->on_update
    );
    return false;
}
#endif
