#include <omega/scene.h>

#if OMG_SUPPORT_SCENES
#include <omega/omega.h>
#include <omega/window.h>
#include <omega/renderer.h>
#define ren ((OMG_Renderer*)this->omg_ren)
#define win_base ((OMG_Window*)this->omg_win)
#define omg_base ((OMG_Omega*)this->omg_omg)
#define SET_EVENT_ARG() ((OMG_Event*)event)->data = this->event_arg
#define CUR_SCENE_CHECK_NULL_VAL(val_name) (OMG_ISNOTNULL(this->cur_scene) && OMG_ISNOTNULL(this->cur_scene->val_name))

// TODO: handle if scene is stopped while updating, painting, etc

void omg_scenemgr_scene_reset_input(OMG_SceneMgr* this, OMG_Scene* scene, bool should_on) {
#if OMG_SCENES_ADV_INPUT
    if (OMG_ISNULL(scene) || !scene->inited)
        return;
    if (((should_on && OMG_ISNOTNULL(scene->on_key_down)) || (!should_on && OMG_ISNOTNULL(scene->on_key_up))))
        for (size_t i = 0; i < 512; i++) {
            if (this->key_states[i] > 0) {
                OMG_EventKeyboard event;
                event.parent.data = omg_base->event_arg;
                event.parent.omg = omg_base;
                event.parent.time = 0; // Why we need this???
                event.code = this->key_states[i];
                event.is_repeated = false;
                event.sym = (uint32_t)i;
                event.win = this->omg_win;
                event.mod = 0; // TODO maybe?
                event.is_pressed = should_on;
                (should_on ? scene->on_key_down : scene->on_key_up)(scene, &event);
            }
        }
    if (((should_on && OMG_ISNOTNULL(scene->on_mouse_down)) || (!should_on && OMG_ISNOTNULL(scene->on_mouse_up))))
        for (size_t i = 0; i < 7; i++) {
            if (this->mouse_states[i].w > 0.0f) {
                OMG_EventMouseButton event;
                event.parent.data = omg_base->event_arg;
                event.parent.omg = omg_base;
                event.parent.time = 0;
                event.button = (uint8_t)i;
                event.clicks = 1;
                event.id = 0; // ?
                event.is_emulated = false; // ?
                event.is_pressed = should_on;
                event.pos.x = this->mouse_states[i].x;
                event.pos.y = this->mouse_states[i].y;
                event.win = win_base;
                event.state = 0;
                (should_on ? scene->on_mouse_down : scene->on_mouse_up)(scene, &event);
            }
        }
    if (((should_on && OMG_ISNOTNULL(scene->on_touch_down)) || (!should_on && OMG_ISNOTNULL(scene->on_touch_up))))
        for (size_t i = 0; i < 64; i++) {
            if (this->finger_states[i].w > 0.0f) {
                OMG_EventTouch event;
                event.parent.data = omg_base->event_arg;
                event.parent.omg = omg_base;
                event.parent.time = 0;
                event.win = win_base;
                event.finger_id = (int64_t)i - 1;
                event.pos.x = this->finger_states[i].x;
                event.pos.y = this->finger_states[i].y;
                event.pressure = 1.0f;
                event.rel.x = event.rel.y = 0.0f;
                event.touch_id = (int64_t)this->finger_states[i].h;
                (should_on ? scene->on_touch_down : scene->on_touch_up)(scene, &event);
            }
        }
#else
    OMG_UNUSED(this, scene, should_on);
#endif
}

bool omg_scenemgr_scene_stop(OMG_SceneMgr* this, OMG_SceneFuncArg* _scene) {
    OMG_Scene* scene = (OMG_Scene*)_scene;
    if (OMG_ISNULL(this->cur_scene) || (OMG_ISNOTNULL(scene) && (scene != this->cur_scene)))
        return false;
    omg_scenemgr_scene_reset_input(this, this->cur_scene, false);
    if (OMG_ISNOTNULL(this->cur_scene->on_stop)) {
        if (scene->on_stop(scene)) {
            _OMG_LOG_ERROR(omg_base, "Failed to stop scene");
        }
    }
    this->cur_scene = NULL;
    return false;
}

bool omg_scenemgr_scene_run(OMG_SceneMgr* this, OMG_SceneFuncArg* _scene) {
    if (OMG_ISNULL(_scene)) {
        _OMG_LOG_WARN(omg_base, "Null pointer passed to omg_scenemgr_scene_run");
        return false;
    }
    OMG_Scene* scene = (OMG_Scene*)_scene;
    omg_scenemgr_scene_stop(this, NULL);
    if (!scene->inited)
        omg_scenemgr_scene_init(this, _scene, scene->data);
    if (OMG_ISNOTNULL(scene->on_run)) {
        if (scene->on_run(scene)) {
            // Lol WTF???
            _OMG_LOG_ERROR(omg_base, "Failed to run scene");
        }
    }
    omg_scenemgr_scene_reset_input(this, this->cur_scene, true);
    scene->dt = 0.0;
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
    scene->on_update = NULL;
    scene->on_paint = NULL;
    scene->on_run = NULL;
    scene->on_stop = NULL;
    scene->on_resize = NULL;
    scene->on_mouse_move = NULL;
    scene->on_mouse_down = NULL;
    scene->on_mouse_up = NULL;
    scene->on_mouse_wheel = NULL;
    scene->on_mouse_enter = NULL;
    scene->on_mouse_leave = NULL;
    scene->on_key_down = NULL;
    scene->on_key_up = NULL;
    scene->on_text_input = NULL;
    scene->on_touch_down = NULL;
    scene->on_touch_up = NULL;
    scene->on_touch_move = NULL;
    scene->dt = 0.0;
    scene->update_on_expose = true;
    scene->paint_on_expose = true;
    scene->enable_paint = true;
    scene->was_allocated = false;
    scene->id = 0;
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
            _OMG_LOG_INFO(omg_base, "OMG scene init failed");
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
    if (this->cur_scene == scene) {
        omg_scenemgr_scene_stop(this, _scene);
    }
    if (OMG_ISNOTNULL(scene->on_destroy))
        scene->on_destroy(scene);
    scene->inited = false;
    if (scene->was_allocated) {
        scene->was_allocated = false;
        OMG_FREE(omg_base->mem, scene);
    }
    return false;    
}

void omg_scenemgr_scene_do_update(OMG_SceneMgr* this) {
    if (OMG_ISNOTNULL(this->cur_scene->on_update)) {
        OMG_Scene* temp_scene = this->cur_scene;
        if (temp_scene->on_update(this->cur_scene)) {
            // Failed? Let's stop drawing current scene
            if (this->cur_scene == temp_scene) {
                omg_scenemgr_scene_stop(this, temp_scene);
            }
        }
    }
}

void omg_scenemgr_event_on_update(OMG_EventUpdate* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (OMG_ISNOTNULL(this->cur_scene))
        omg_scenemgr_scene_do_update(this);
    this->on_update(event);
}

void omg_scenemgr_scene_do_paint(OMG_SceneMgr* this) {
    if (OMG_ISNOTNULL(this->cur_scene->on_paint)) {
        OMG_Scene* temp_scene = this->cur_scene;
        if (temp_scene->enable_paint && temp_scene->on_paint(temp_scene)) {
            if (this->cur_scene == temp_scene) {
                omg_scenemgr_scene_stop(this, temp_scene);
            }
        }
    }
}

void omg_scenemgr_event_on_paint(OMG_EventPaint* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        if (OMG_ISNOTNULL(this->cur_scene))
            omg_scenemgr_scene_do_paint(this);
    }
    this->on_paint(event);
}

void omg_scenemgr_event_on_expose(OMG_EventExpose* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        if (OMG_ISNOTNULL(this->cur_scene) && this->cur_scene->update_on_expose)
            omg_scenemgr_scene_do_update(this);
        if (OMG_ISNOTNULL(this->cur_scene) && this->cur_scene->paint_on_expose)
            omg_scenemgr_scene_do_paint(this);
    }
    this->on_expose(event);
}

void omg_scenemgr_event_on_size_change(OMG_EventResize* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        if (CUR_SCENE_CHECK_NULL_VAL(on_resize)) {
            this->cur_scene->on_resize(this->cur_scene, event);
        }
    }
    this->on_size_change(event);
}

void omg_scenemgr_event_on_loop_stop(OMG_EventLoopStop* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    /*if (OMG_ISNOTNULL(this->cur_scene) && this->cur_scene->inited) {
        omg_scenemgr_scene_stop(this, this->cur_scene);
    }*/
    this->on_loop_stop(event);
}

void omg_scenemgr_event_on_mouse_wheel(OMG_EventMouseWheel* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        if (CUR_SCENE_CHECK_NULL_VAL(on_mouse_wheel)) {
            this->cur_scene->on_mouse_wheel(this->cur_scene, event);
        }
    }
    this->on_mouse_wheel(event);
}

void omg_scenemgr_event_on_mouse_move(OMG_EventMouseMove* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
#if OMG_SCENES_ADV_INPUT
    // How?
    /* if (event->button <= 7) {
        this->mouse_states[event->button].x = event->pos.x;
        this->mouse_states[event->button].y = event->pos.y;
    } */
    for (size_t i = 0; i < 8; i++) {
        this->mouse_states[i].x = event->pos.x;
        this->mouse_states[i].y = event->pos.y;
    }
#endif
    if (event->win == this->omg_win) {
        this->is_mouse_entered = true;
        if (CUR_SCENE_CHECK_NULL_VAL(on_mouse_move)) {
            this->cur_scene->on_mouse_move(this->cur_scene, event);
        }
    }
    this->on_mouse_move(event);
}

void omg_scenemgr_event_on_mouse_down(OMG_EventMouseButton* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
#if OMG_SCENES_ADV_INPUT
    if (event->button <= 7) {
        this->mouse_states[event->button].x = event->pos.x;
        this->mouse_states[event->button].y = event->pos.y;
        this->mouse_states[event->button].w = 1337.0f;
    }
#endif
    if (event->win == this->omg_win) {
        if (CUR_SCENE_CHECK_NULL_VAL(on_mouse_down)) {
            this->cur_scene->on_mouse_down(this->cur_scene, event);
        }
    }
    this->on_mouse_down(event);
}

void omg_scenemgr_event_on_mouse_up(OMG_EventMouseButton* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
#if OMG_SCENES_ADV_INPUT
    if (event->button <= 7) {
        this->mouse_states[event->button].x = event->pos.x;
        this->mouse_states[event->button].y = event->pos.y;
        this->mouse_states[event->button].w = 0.0f;
    }
#endif
    if (event->win == this->omg_win) {
        if (CUR_SCENE_CHECK_NULL_VAL(on_mouse_up)) {
            this->cur_scene->on_mouse_up(this->cur_scene, event);
        }
    }
    this->on_mouse_up(event);
}

void omg_scenemgr_event_on_mouse_enter(OMG_EventMouseFocus* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        this->is_mouse_entered = true;
        if (CUR_SCENE_CHECK_NULL_VAL(on_mouse_enter)) {
            this->cur_scene->on_mouse_enter(this->cur_scene, event);
        }
    }
    this->on_mouse_enter(event);
}

void omg_scenemgr_event_on_mouse_leave(OMG_EventMouseFocus* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        this->is_mouse_entered = false;
        if (CUR_SCENE_CHECK_NULL_VAL(on_mouse_leave)) {
            this->cur_scene->on_mouse_leave(this->cur_scene, event);
        }
    }
    this->on_mouse_leave(event);
}

void omg_scenemgr_event_on_key_down(OMG_EventKeyboard* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
#if OMG_SCENES_ADV_INPUT
    if (event->sym < 520)
        this->key_states[event->sym] = event->code;
#endif
    if (event->win == this->omg_win) {
        if (CUR_SCENE_CHECK_NULL_VAL(on_key_down)) {
            this->cur_scene->on_key_down(this->cur_scene, event);
        }
    }
    this->on_key_down(event);
}

void omg_scenemgr_event_on_key_up(OMG_EventKeyboard* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
#if OMG_SCENES_ADV_INPUT
    if (event->sym < 520)
        this->key_states[event->sym] = 0;
#endif
    if (event->win == this->omg_win) {
        if (CUR_SCENE_CHECK_NULL_VAL(on_key_up)) {
            this->cur_scene->on_key_up(this->cur_scene, event);
        }
    }
    this->on_key_up(event);
}

void omg_scenemgr_event_on_text_input(OMG_EventTextInput* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
    if (event->win == this->omg_win) {
        if (CUR_SCENE_CHECK_NULL_VAL(on_text_input)) {
            this->cur_scene->on_text_input(this->cur_scene, event);
        }
    }
    this->on_text_input(event);
}

void omg_scenemgr_event_on_touch_down(OMG_EventTouch* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
#if OMG_SCENES_ADV_INPUT
    if ((event->finger_id < 63) && (event->finger_id >= -1)) {
        this->mouse_states[event->finger_id + 1].x = event->pos.x;
        this->mouse_states[event->finger_id + 1].y = event->pos.y;
        this->mouse_states[event->finger_id + 1].w = 1337.0f;
        this->mouse_states[event->finger_id + 1].h = (float)event->touch_id;
    }
#endif
    if (event->win == this->omg_win) {
        if (CUR_SCENE_CHECK_NULL_VAL(on_touch_down)) {
            this->cur_scene->on_touch_down(this->cur_scene, event);
        }
    }
    this->on_touch_down(event);
}

void omg_scenemgr_event_on_touch_up(OMG_EventTouch* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
#if OMG_SCENES_ADV_INPUT
    if ((event->finger_id < 63) && (event->finger_id >= -1)) {
        this->mouse_states[event->finger_id + 1].x = event->pos.x;
        this->mouse_states[event->finger_id + 1].y = event->pos.y;
        this->mouse_states[event->finger_id + 1].w = 0.0f;
    }
#endif
    if (event->win == this->omg_win) {
        if (CUR_SCENE_CHECK_NULL_VAL(on_touch_up)) {
            this->cur_scene->on_touch_up(this->cur_scene, event);
        }
    }
    this->on_touch_up(event);
}

void omg_scenemgr_event_on_touch_move(OMG_EventTouch* event) {
    OMG_SceneMgr* this = OMG_ARG_FROM_EVENT(event);
    SET_EVENT_ARG();
#if OMG_SCENES_ADV_INPUT
    if ((event->finger_id < 63) && (event->finger_id >= -1)) {
        this->mouse_states[event->finger_id + 1].x = event->pos.x;
        this->mouse_states[event->finger_id + 1].y = event->pos.y;
    }
#endif
    if (event->win == this->omg_win) {
        if (CUR_SCENE_CHECK_NULL_VAL(on_touch_move)) {
            this->cur_scene->on_touch_move(this->cur_scene, event);
        }
    }
    this->on_touch_move(event);
}

bool omg_scenemgr_init(OMG_SceneMgr* this, void* omg_ren) {
    this->omg_ren = omg_ren;
    this->omg_omg = ren->omg;
    this->omg_win = ren->win;
    this->is_mouse_entered = true;
#if OMG_SCENES_ADV_INPUT
    omg_base->std->memset(this->key_states, 0, sizeof(this->key_states));
    omg_base->std->memset(this->mouse_states, 0, sizeof(this->mouse_states));
    omg_base->std->memset(this->finger_states, 0, sizeof(this->finger_states));
#endif
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
    if (OMG_ISNOTNULL(this->cur_scene) && this->cur_scene->inited) {
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
