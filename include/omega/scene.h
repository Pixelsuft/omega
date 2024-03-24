#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_SCENES
#include <omega/ostd.h>
#include <omega/omega.h>
#define OMG_SceneFuncArg void

#define OMG_SCENEMGR_FROM_SCENE(scene) ((OMG_SceneMgr*)(((OMG_Scene*)scene)->omg_scenemgr))

struct OMG_SceneMgr;
struct OMG_Scene;

typedef struct OMG_Object {
    OMG_FRect rect;
    bool (*on_update)(struct OMG_Object* this, struct OMG_Scene* scene);
    bool (*on_paint)(struct OMG_Object* this, struct OMG_Scene* scene);
} OMG_Object;

typedef struct OMG_Scene {
    OMG_Object parent;
    void* unused1;
    void* unused2;
    void* unused3;
    void* omg_scenemgr;
    void* data;
    // TODO: where is OMG_EventPaint or I don't need it?
    // Scene init event
    bool (*on_init)(struct OMG_Scene* this);
    // Scene destroy event
    bool (*on_destroy)(struct OMG_Scene* this);
    // Scene update event
    bool (*on_update)(struct OMG_Scene* this);
    // Scene paint event
    bool (*on_paint)(struct OMG_Scene* this);
    // Scene run event
    bool (*on_run)(struct OMG_Scene* this);
    // Scene stop event
    bool (*on_stop)(struct OMG_Scene* this);
    void (*on_resize)(struct OMG_Scene* this, OMG_EventResize* event);
    void (*on_mouse_move)(struct OMG_Scene* this, OMG_EventMouseMove* event);
    void (*on_mouse_down)(struct OMG_Scene* this, OMG_EventMouseButton* event);
    void (*on_mouse_up)(struct OMG_Scene* this, OMG_EventMouseButton* event);
    void (*on_mouse_wheel)(struct OMG_Scene* this, OMG_EventMouseWheel* event);
    void (*on_mouse_enter)(struct OMG_Scene* this, OMG_EventMouseFocus* event);
    void (*on_mouse_leave)(struct OMG_Scene* this, OMG_EventMouseFocus* event);
    void (*on_key_down)(struct OMG_Scene* this, OMG_EventKeyboard* event);
    void (*on_key_up)(struct OMG_Scene* this, OMG_EventKeyboard* event);
    void (*on_text_input)(struct OMG_Scene* this, OMG_EventTextInput* event);
    void (*on_touch_down)(struct OMG_Scene* this, OMG_EventTouch* event);
    void (*on_touch_up)(struct OMG_Scene* this, OMG_EventTouch* event);
    void (*on_touch_move)(struct OMG_Scene* this, OMG_EventTouch* event);
    double dt;
    int id;
    bool was_allocated;
    bool update_on_expose;
    bool paint_on_expose;
    bool enable_paint;
    bool reset_input;
    bool inited;
} OMG_Scene;

typedef struct OMG_SceneMgr {
    void* omg_ren;
    void* omg_win;
    OMG_Omega* omg_omg;
    OMG_Scene* cur_scene;
#if OMG_SCENES_ADV_INPUT
    uint32_t key_states[524];
    OMG_FRect mouse_states[8];
    OMG_FRect finger_states[64];
#endif
    void* event_arg;
    void (*on_update)(OMG_EventUpdate* event);
    void (*on_loop_stop)(OMG_EventLoopStop* event);
    void (*on_paint)(OMG_EventPaint* event);
    void (*on_expose)(OMG_EventExpose* event);
    void (*on_size_change)(OMG_EventResize* event);
    void (*on_mouse_move)(OMG_EventMouseMove* event);
    void (*on_mouse_down)(OMG_EventMouseButton* event);
    void (*on_mouse_up)(OMG_EventMouseButton* event);
    void (*on_mouse_wheel)(OMG_EventMouseWheel* event);
    void (*on_mouse_enter)(OMG_EventMouseFocus* event);
    void (*on_mouse_leave)(OMG_EventMouseFocus* event);
    void (*on_key_down)(OMG_EventKeyboard* event);
    void (*on_key_up)(OMG_EventKeyboard* event);
    void (*on_text_input)(OMG_EventTextInput* event);
    void (*on_touch_down)(OMG_EventTouch* event);
    void (*on_touch_up)(OMG_EventTouch* event);
    void (*on_touch_move)(OMG_EventTouch* event);
    bool is_mouse_entered;
} OMG_SceneMgr;

// Init scene manager (fill it before doing init)
OMG_API bool omg_scenemgr_init(OMG_SceneMgr* this, void* omg_ren);
// Fill scene manager (before init)
OMG_API bool omg_scenemgr_scene_fill(OMG_SceneMgr* this, OMG_SceneFuncArg* _scene);
// Destroy scene manager
OMG_API bool omg_scenemgr_destroy(OMG_SceneMgr* this);
// Run scene
OMG_API bool omg_scenemgr_scene_run(OMG_SceneMgr* this, OMG_SceneFuncArg* scene);
// Stop scene
OMG_API bool omg_scenemgr_scene_stop(OMG_SceneMgr* this, OMG_SceneFuncArg* scene);
// Init scene
OMG_API bool omg_scenemgr_scene_init(OMG_SceneMgr* this, OMG_SceneFuncArg* scene, void* data);
// Destroy scene
OMG_API bool omg_scenemgr_scene_destroy(OMG_SceneMgr* this, OMG_SceneFuncArg* scene);
#if OMG_EXPORT_SHIT
OMG_API void omg_scenemgr_scene_reset_input(OMG_SceneMgr* this, OMG_Scene* scene, bool should_on);
OMG_API void omg_scenemgr_scene_do_paint(OMG_SceneMgr* this);
OMG_API void omg_scenemgr_scene_do_update(OMG_SceneMgr* this);
#endif
#endif
