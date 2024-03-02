#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_SCENES
#include <omega/ostd.h>
#include <omega/scene.h>

typedef struct {
    OMG_Object base;
    OMG_Clock* clock;
    double counter;
    double duration;
    int triggered;
    bool soft;
    bool repeat;
    bool running;
} OMG_ObjectTimer;

typedef struct {
    OMG_Object base;
    OMG_Clock* clock;
    double time;
    double duration;
    bool soft;
    bool running;
    bool repeat;
} OMG_ObjectAnimTimer;

OMG_API bool omg_obj_timer_init(OMG_ObjectTimer* this, OMG_Omega* omg);
OMG_API bool omg_obj_anim_timer_init(OMG_ObjectAnimTimer* this, OMG_Omega* omg);
#if OMG_EXPORT_SHIT
OMG_API bool omg_obj_timer_on_update(OMG_ObjectTimer* this, OMG_Scene* scene);
OMG_API bool omg_obj_anim_timer_on_update(OMG_ObjectAnimTimer* this, OMG_Scene* scene);
#endif
#endif
