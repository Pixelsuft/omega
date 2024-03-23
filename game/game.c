#include <game.h>
#include <aamain.h>
#define base ((OMG_Scene*)this)
#define rn app->ren

bool game_scene_on_update(GameScene* this) {
    App* app = base->data;
    app->clock->update(app->clock);
    base->dt = app->clock->dt;
    OMG_BEGIN_POINTER_CAST();
    OMG_END_POINTER_CAST();
    return false;
}

bool game_scene_on_paint(GameScene* this) {
    App* app = base->data;
    rn->begin(rn);
    rn->clear(rn, &OMG_RGB(0, 0, 0));
    rn->set_scale(rn, NULL, &OMG_FPOINT(app->sc.w / 4.0f, app->sc.h / 4.0f));
    app_draw_fps(app);
    rn->set_scale(rn, NULL, &OMG_FPOINT(app->sc.w, app->sc.h));
    rn->flip(rn);
    return false;
}

bool game_scene_on_run(GameScene* this) {
    App* app = base->data;
    app->clock->reset(app->clock);
    return false;
}

void game_scene_on_resize(GameScene* this, OMG_EventResize* event) {
    App* app = base->data;
    rn->set_scale(rn, NULL, &app->sc);
}

void game_scene_on_keyboard(GameScene* this, OMG_EventKeyboard* event) {
    App* app = base->data;
    if (IS_BACK_CODE(event->code) || IS_EXIT_CODE(event->code)) {
        omg_scenemgr_scene_destroy(&app->sm, this);
        app->omg->auto_loop_stop(app->omg);
    }
}

bool game_scene_on_destroy(GameScene* this) {
    App* app = base->data;
    return false;
}

bool game_scene_on_stop(GameScene* this) {
    App* app = base->data;
    return false;
}

bool game_scene_init(GameScene* this) {
    App* app = base->data;
    OMG_BEGIN_POINTER_CAST();
    base->on_run = game_scene_on_run;
    base->on_update = game_scene_on_update;
    base->on_paint = game_scene_on_paint;
    base->on_resize = game_scene_on_resize;
    base->on_key_down = game_scene_on_keyboard;
    base->on_destroy = game_scene_on_destroy;
    base->on_stop = game_scene_on_stop;
    OMG_END_POINTER_CAST();
    base->inited = true;
    base->was_allocated = true;
    return false;
}
