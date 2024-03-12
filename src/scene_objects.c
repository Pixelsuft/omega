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

bool omg_anim_sprite_state_destroy(OMG_AnimSpriteState* this, OMG_Omega* omg) {
    if (OMG_ISNULL(this->durations))
        return false;
    OMG_FREE(omg->mem, this->durations);
    this->durations = NULL;
    this->base_id = 0;
    this->num_frames = 0;
    return false;
}

bool omg_anim_sprite_state_init(OMG_AnimSpriteState* this, OMG_Omega* omg, double default_duration, int num_frames) {
    this->base_id = 0;
    this->num_frames = 0;
    this->durations = OMG_MALLOC(omg->mem, sizeof(double) * num_frames);
    if (OMG_ISNULL(this->durations)) {
        _OMG_LOG_ERROR(omg, "Failed to init durations array");
        return true;
    }
    this->num_frames = num_frames;
    for (int i = 0; i < num_frames; i++) {
        this->durations[i] = default_duration;
    }
    return false;
}

bool omg_obj_anim_sprite_on_update(OMG_ObjectAnimSprite* this, OMG_Scene* scene) {
    OMG_UNUSED(this, scene);
    // TODO
    return false;
}

bool omg_obj_anim_sprite_init(OMG_ObjectAnimSprite* this) {
    if (OMG_ISNULL(this->data)) {
        // _OMG_LOG_ERROR(omg_base, "Passed NULL data for omg_obj_anim_sprite_init");
        this->parent.on_paint = NULL;
        this->parent.on_update = NULL;
        this->parent.rect.x = this->parent.rect.y = this->parent.rect.w = this->parent.rect.h = 0.0f;
        this->running = false;
        this->soft = false;
        this->data = NULL;
        return true;
    }
    this->parent.on_paint = NULL;
    OMG_BEGIN_POINTER_CAST();
    this->parent.on_update = omg_obj_anim_sprite_on_update;
    OMG_END_POINTER_CAST();
    this->parent.rect.x = this->parent.rect.y = this->parent.rect.w = this->parent.rect.h = 0.0f;
    this->running = false;
    this->soft = false;
    int cur_base_id = 0;
    for (size_t i = 0; i < this->data->states.len; i++) {
        this->data->states.data[i].base_id = cur_base_id;
        cur_base_id += this->data->states.data[i].num_frames;
    }
    return false;
}

bool omg_anim_sprite_data_destroy(OMG_AnimSpriteData* this) {
    if (OMG_ISNULL(this->states.data))
        return false;
    return OMG_ARRAY_DESTROY(&this->states);
}

bool omg_anim_sprite_data_init(OMG_AnimSpriteData* this, OMG_Omega* omg) {
    if (OMG_ARRAY_INIT(&this->states, 0, 0)) {
        _OMG_LOG_ERROR(omg, "Failed to init sprite data for animation");
        return true;
    }
    return false;
}
#endif
