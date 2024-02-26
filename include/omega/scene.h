#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_SCENES
#include <omega/ostd.h>
#include <omega/omega.h>
#define OMG_SceneFuncArg void

typedef struct {
    OMG_FRect rect;
} OMG_Object;

typedef struct OMG_Scene {
    OMG_Object parent;
    void* unused1;
    void* unused2;
    void* unused3;
    void* omg_scenemgr;
    void* data;
    bool (*on_init)(struct OMG_Scene* this);
    bool (*on_destroy)(struct OMG_Scene* this);
    int dummy;
    bool inited;
} OMG_Scene;

typedef struct OMG_SceneMgr {
    void* omg_ren;
    void* omg_win;
    OMG_Omega* omg_omg;
    OMG_Scene* cur_scene;
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

OMG_API bool omg_scenemgr_init(OMG_SceneMgr* this, void* omg_ren);
OMG_API bool omg_scenemgr_scene_fill(OMG_SceneMgr* this, OMG_SceneFuncArg* _scene);
OMG_API bool omg_scenemgr_destroy(OMG_SceneMgr* this);
OMG_API bool omg_scenemgr_scene_run(OMG_SceneMgr* this, OMG_SceneFuncArg* scene);
OMG_API bool omg_scenemgr_scene_stop(OMG_SceneMgr* this, OMG_SceneFuncArg* scene);
OMG_API bool omg_scenemgr_scene_init(OMG_SceneMgr* this, OMG_SceneFuncArg* scene, void* data);
OMG_API bool omg_scenemgr_scene_destroy(OMG_SceneMgr* this, OMG_SceneFuncArg* scene);
#if OMG_EXPORT_SHIT
#endif
#endif
