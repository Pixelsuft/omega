#pragma once
#include <omega/config.h>
#if OMG_SUPPORT_SCENES
#include <omega/ostd.h>
#include <omega/scene.h>
#include <omega/array.h>

typedef struct {
    OMG_Object parent;
    double counter;
    double duration;
    int triggered;
    bool soft;
    bool repeat;
    bool running;
} OMG_ObjectTimer;

typedef struct {
    OMG_Object parent;
    double time;
    double duration;
    bool soft;
    bool running;
    bool repeat;
} OMG_ObjectAnimTimer;

typedef struct {
    double* durations;
    int num_frames;
    int base_id;
} OMG_AnimSpriteState;

typedef struct {
    OMG_Array(OMG_AnimSpriteState) states;
} OMG_AnimSpriteData;

typedef struct {
    OMG_Object parent;
    OMG_AnimSpriteData* data;
    double cur_timer;
    int cur_frame;
    int cur_state;
    int cur_base_id;
    int cur_id;
    bool running;
} OMG_ObjectAnimSprite;

OMG_API bool omg_obj_timer_init(OMG_ObjectTimer* this, OMG_Omega* omg);
OMG_API bool omg_obj_anim_timer_init(OMG_ObjectAnimTimer* this, OMG_Omega* omg);
OMG_API bool omg_anim_sprite_state_init(OMG_AnimSpriteState* this, OMG_Omega* omg, double default_duration, int num_frames);
OMG_API bool omg_anim_sprite_state_destroy(OMG_AnimSpriteState* this, OMG_Omega* omg);
OMG_API bool omg_anim_sprite_data_init(OMG_AnimSpriteData* this, OMG_Omega* omg);
OMG_API bool omg_anim_sprite_data_destroy(OMG_AnimSpriteData* this, OMG_Omega* omg, bool destroy_states);
OMG_API bool omg_obj_anim_sprite_init(OMG_ObjectAnimSprite* this);
OMG_API bool omg_obj_anim_run_state(OMG_ObjectAnimSprite* this, int state_id);
#if OMG_EXPORT_SHIT
OMG_API bool omg_obj_anim_sprite_on_update(OMG_ObjectAnimSprite* this, OMG_Scene* scene);
OMG_API bool omg_obj_timer_on_update(OMG_ObjectTimer* this, OMG_Scene* scene);
OMG_API bool omg_obj_anim_timer_on_update(OMG_ObjectAnimTimer* this, OMG_Scene* scene);
#endif
#endif
