#include <omega/scene_objects.h>

#if OMG_SUPPORT_SCENES
#define obj_base ((OMG_Object*)this)

bool omg_obj_timer_on_update(OMG_Scene* scene, OMG_ObjectTimer* this) {
    OMG_UNUSED(this, scene);
    return false;
}

bool omg_obj_timer_init(OMG_ObjectTimer* this, OMG_Omega* omg) {
    this->clock = omg->clock;
    this->duration = 0.0;
    this->counter = 0.0;
    this->repeat = true;
    this->running = false;
    OMG_BEGIN_POINTER_CAST();
    obj_base->on_update = omg_obj_timer_on_update;
    obj_base->on_paint = NULL;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
