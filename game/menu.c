#include <menu.h>
#include <aamain.h>
#define base ((OMG_Scene*)this)
#define rn app->ren

bool menu_scene_on_update(MenuScene* this) {
    App* app = base->data;
    app->clock->update(app->clock);
    base->dt = app->clock->dt;
    return false;
}

bool menu_scene_on_paint(MenuScene* this) {
    App* app = base->data;
    rn->begin(rn);
    rn->clear(rn, &OMG_RGB(0, 255, 0));
    rn->flip(rn);
    return false;
}

bool menu_scene_on_run(MenuScene* this) {
    App* app = base->data;
    app->clock->reset(app->clock);
    return false;
}

void menu_scene_on_resize(MenuScene* this, OMG_EventResize* event) {
    App* app = base->data;
    rn->set_scale(rn, NULL, &app->sc);
}

void menu_scene_on_keyboard(MenuScene* this, OMG_EventKeyboard* event) {
    App* app = base->data;
}

bool menu_scene_on_destroy(MenuScene* this) {
    App* app = base->data;
    return false;
}

bool menu_scene_on_stop(MenuScene* this) {
    App* app = base->data;
    return false;
}

bool menu_scene_init(MenuScene* this) {
    App* app = base->data;
    OMG_BEGIN_POINTER_CAST();
    base->on_run = menu_scene_on_run;
    base->on_update = menu_scene_on_update;
    base->on_paint = menu_scene_on_paint;
    base->on_resize = menu_scene_on_resize;
    base->on_key_down = menu_scene_on_keyboard;
    base->on_destroy = menu_scene_on_destroy;
    base->on_stop = menu_scene_on_stop;
    OMG_END_POINTER_CAST();
    base->inited = true;
    base->was_allocated = true;
    return false;
}
