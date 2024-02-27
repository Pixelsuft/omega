#include <omega/scene.h>

#if OMG_SUPPORT_SCENES
#include <omega/omega.h>
#include <omega/window.h>
#include <omega/renderer.h>
#define ren ((OMG_Renderer*)this->omg_ren)
#define omg_base ((OMG_Omega*)this->omg_omg)
#define SET_EVENT_ARG() ((OMG_Event*)event)->data = this->event_arg

bool omg_scenemgr_scene_stop(OMG_SceneMgr* this, OMG_SceneFuncArg* _scene) {
    OMG_Scene* scene = (OMG_Scene*)_scene;
    if (OMG_ISNULL(this->cur_scene) || (OMG_ISNOTNULL(scene) && (scene != this->cur_scene)))
        return false;
    this->cur_scene = NULL;
    return false;
}

bool omg_scenemgr_scene_run(OMG_SceneMgr* this, OMG_SceneFuncArg* _scene) {
    OMG_Scene* scene = (OMG_Scene*)_scene;
    omg_scenemgr_scene_stop(this, NULL);
    if (!scene->inited)
        omg_scenemgr_scene_init(this, _scene, scene->data);
    this->cur_scene = scene;
    return false;
}

bool omg_scenemgr_scene_fill(OMG_SceneMgr* this, OMG_SceneFuncArg* _scene) {
    if (OMG_ISNULL(_scene)) {
        _OMG_LOG_WARN(omg_base, "Null pointer passed to omg_scenemgr_scene_fill");
        return false;
    }
    OMG_Scene* scene = (OMG_Scene*)_scene;
    scene->inited = false;
    scene->unused1 = scene->unused2 = scene->unused3 = NULL;
    scene->omg_scenemgr = this;
    scene->on_init = NULL;
    scene->on_destroy = NULL;
    return false;
}

bool omg_scenemgr_scene_init(OMG_SceneMgr* this, OMG_SceneFuncArg* _scene, void* data) {
    if (OMG_ISNULL(_scene)) {
        _OMG_LOG_WARN(omg_base, "Null pointer passed to omg_scenemgr_scene_init");
        return false;
    }
    OMG_Scene* scene = (OMG_Scene*)_scene;
    scene->inited = true;
    scene->data = data;
    if (OMG_ISNOTNULL(scene->on_init)) {
        if (scene->on_init(scene)) {
            // ?
            _OMG_LOG_INFO(omg_base, "Lol scene init failed");
            return true;
        }
    }
    return false;
}

bool omg_scenemgr_scene_destroy(OMG_SceneMgr* this, OMG_SceneFuncArg* _scene) {
    if (OMG_ISNULL(_scene)) {
        _OMG_LOG_WARN(omg_base, "Null pointer passed to omg_scenemgr_scene_destroy");
        return false;
    }
    OMG_Scene* scene = (OMG_Scene*)_scene;
    if (!scene->inited)
        return false;
    if (OMG_ISNOTNULL(scene->on_destroy))
        scene->on_destroy(scene);
    scene->inited = false;
    OMG_UNUSED(this);
    return false;    
}

void omg_scenemgr_event_on_update(OMG_EventUpdate* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    this->on_update(event);
}

void omg_scenemgr_event_on_paint(OMG_EventPaint* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {

    }
    this->on_paint(event);
}

void omg_scenemgr_event_on_expose(OMG_EventExpose* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        
    }
    this->on_expose(event);
}

void omg_scenemgr_event_on_size_change(OMG_EventResize* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        
    }
    this->on_size_change(event);
}

void omg_scenemgr_event_on_loop_stop(OMG_EventLoopStop* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    this->on_loop_stop(event);
}

void omg_scenemgr_event_on_mouse_wheel(OMG_EventMouseWheel* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        
    }
    this->on_mouse_wheel(event);
}

void omg_scenemgr_event_on_mouse_move(OMG_EventMouseMove* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        this->is_mouse_entered = true;        
    }
    this->on_mouse_move(event);
}

void omg_scenemgr_event_on_mouse_down(OMG_EventMouseButton* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        
    }
    this->on_mouse_down(event);
}

void omg_scenemgr_event_on_mouse_up(OMG_EventMouseButton* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        
    }
    this->on_mouse_up(event);
}

void omg_scenemgr_event_on_mouse_enter(OMG_EventMouseFocus* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        this->is_mouse_entered = true;
    }
    this->on_mouse_enter(event);
}

void omg_scenemgr_event_on_mouse_leave(OMG_EventMouseFocus* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        this->is_mouse_entered = false;
    }
    this->on_mouse_leave(event);
}

void omg_scenemgr_event_on_key_down(OMG_EventKeyboard* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        
    }
    this->on_key_down(event);
}

void omg_scenemgr_event_on_key_up(OMG_EventKeyboard* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        
    }
    this->on_key_up(event);
}

void omg_scenemgr_event_on_text_input(OMG_EventTextInput* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        
    }
    this->on_text_input(event);
}

void omg_scenemgr_event_on_touch_down(OMG_EventTouch* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        
    }
    this->on_touch_down(event);
}

void omg_scenemgr_event_on_touch_up(OMG_EventTouch* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        
    }
    this->on_touch_up(event);
}

void omg_scenemgr_event_on_touch_move(OMG_EventTouch* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        
    }
    this->on_touch_move(event);
}

bool omg_scenemgr_init(OMG_SceneMgr* this, void* omg_ren) {
    this->omg_ren = omg_ren;
    this->omg_omg = ren->omg;
    this->omg_win = ren->win;
    this->is_mouse_entered = true;
    // omg_base->std->memset((void*)((size_t)this->on_update), 0, (size_t)this->on_touch_move - (size_t)this->on_update);
    omg_base->std->memcpy(
        (void*)((size_t)(&this->on_update)),
        (void*)((size_t)(&omg_base->on_update)),
        (size_t)(&this->on_touch_move) - (size_t)(&this->on_update)
    );
    omg_base->on_update = omg_scenemgr_event_on_update;
    omg_base->on_loop_stop = omg_scenemgr_event_on_loop_stop;
    omg_base->on_paint = omg_scenemgr_event_on_paint;
    omg_base->on_expose = omg_scenemgr_event_on_expose;
    omg_base->on_size_change = omg_scenemgr_event_on_size_change;
    omg_base->on_mouse_move = omg_scenemgr_event_on_mouse_move;
    omg_base->on_mouse_down = omg_scenemgr_event_on_mouse_down;
    omg_base->on_mouse_up = omg_scenemgr_event_on_mouse_up;
    omg_base->on_mouse_wheel = omg_scenemgr_event_on_mouse_wheel;
    omg_base->on_mouse_enter = omg_scenemgr_event_on_mouse_enter;
    omg_base->on_mouse_leave = omg_scenemgr_event_on_mouse_leave;
    omg_base->on_key_down = omg_scenemgr_event_on_key_down;
    omg_base->on_key_up = omg_scenemgr_event_on_key_up;
    omg_base->on_text_input = omg_scenemgr_event_on_text_input;
    omg_base->on_touch_down = omg_scenemgr_event_on_touch_down;
    omg_base->on_touch_up = omg_scenemgr_event_on_touch_up;
    omg_base->on_touch_move = omg_scenemgr_event_on_touch_move;
    this->event_arg = omg_base->event_arg;
    omg_base->event_arg = this;
    this->cur_scene = NULL;
    _OMG_LOG_INFO(omg_base, "Omega scenes successfully inited");
    return false;
}

bool omg_scenemgr_destroy(OMG_SceneMgr* this) {
    if (OMG_ISNOTNULL(this->cur_scene)) {
        omg_scenemgr_scene_destroy(this, this->cur_scene);
    }
    omg_base->std->memcpy(
        (void*)((size_t)(&omg_base->on_update)),
        (void*)((size_t)(&this->on_update)),
        (size_t)(&this->on_touch_move) - (size_t)(&this->on_update)
    );
    omg_base->event_arg = this->event_arg;
    return false;
}
#endif