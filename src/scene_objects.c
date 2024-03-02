#include <omega/scene_objects.h>

#if OMG_SUPPORT_SCENES
#define obj_base ((OMG_Object*)this)

bool omg_obj_timer_on_update(OMG_ObjectTimer* this, OMG_Scene* scene) {
    OMG_UNUSED(scene);
    if (!this->running)
        return false;
    this->counter += this->clock->dt;
    if (this->soft) {
        if (this->counter >= this->duration) {
            this->counter -= this->duration;
            this->triggered++;
            if (!this->repeat)
                this->running = false;
        }
    }
    else {
        while (this->counter >= this->duration) {
            this->counter -= this->duration;
            this->triggered++;
            if (!this->repeat) {
                this->running = false;
                break;
            }
        }
    }
    return false;
}

bool omg_obj_timer_init(OMG_ObjectTimer* this, OMG_Omega* omg) {
    this->clock = omg->clock;
    this->duration = 0.0;
    this->counter = 0.0;
    this->triggered = 0;
    this->repeat = true;
    this->running = false;
    this->soft = false;
    OMG_BEGIN_POINTER_CAST();
    obj_base->on_update = omg_obj_timer_on_update;
    obj_base->on_paint = NULL;
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_obj_anim_timer_on_update(OMG_ObjectAnimTimer* this, OMG_Scene* scene) {
    OMG_UNUSED(scene);
    if (!this->running)
        return false;
    this->time += this->clock->dt;
    if (this->soft) {
        if (this->time >= this->duration) {
            this->time -= this->duration;
            if (!this->repeat)
                this->running = false;
        }
    }
    else {
        while (this->time >= this->duration) {
            this->time -= this->duration;
            if (!this->repeat) {
                this->running = false;
                break;
            }
        }
    }
    return false;
}

bool omg_obj_anim_timer_init(OMG_ObjectAnimTimer* this, OMG_Omega* omg) {
    this->clock = omg->clock;
    this->duration = 0.0;
    this->time = 0.0;
    this->repeat = true;
    this->running = false;
    this->soft = false;
    OMG_BEGIN_POINTER_CAST();
    obj_base->on_update = omg_obj_anim_timer_on_update;
    obj_base->on_paint = NULL;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
