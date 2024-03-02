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
    bool repeat;
    bool running;
} OMG_ObjectTimer;

OMG_API bool omg_obj_timer_init(OMG_ObjectTimer* this, OMG_Omega* omg);
#if OMG_EXPORT_SHIT
OMG_API bool omg_obj_timer_on_update(OMG_ObjectTimer* this, OMG_Scene* scene);
#endif
#endif
