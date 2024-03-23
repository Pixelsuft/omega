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
    rn->clear(rn, &OMG_RGB(0, 0, 0));
    rn->copy_ex(rn, this->bg, NULL, NULL, NULL, 0.0);
    rn->set_scale(rn, NULL, &OMG_FPOINT(app->sc.w / 2.0f, app->sc.h / 2.0f));
    omg_bmfont_render(&app->ld.fnt[0], &OMG_STR("TEST GAME!!!"), &OMG_FPOINT(290, 100));
    rn->set_scale(rn, NULL, &OMG_FPOINT(app->sc.w, app->sc.h));
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
    if (IS_BACK_CODE(event->code) || IS_EXIT_CODE(event->code)) {
        omg_scenemgr_scene_destroy(&app->sm, this);
        app->omg->auto_loop_stop(app->omg);
    }
}

bool menu_scene_on_destroy(MenuScene* this) {
    App* app = base->data;
    rn->tex_destroy(rn, this->bg);
    return false;
}

bool menu_scene_on_stop(MenuScene* this) {
    App* app = base->data;
    return false;
}

bool menu_scene_init(MenuScene* this) {
    App* app = base->data;
    rn->tex_set_scale_mode(rn, app->ld.fnt[0].page, OMG_SCALE_MODE_LINEAR);
    this->bg = rn->tex_create(rn, NULL, &OMG_FPOINT(800, 600), OMG_TEXTURE_ACCESS_TARGET, false);
    rn->set_target(rn, this->bg);
    rn->clear(rn, &OMG_RGB(0, 0, 0));
    for (float i = 0.0f; i < 800.0f; i += 20.0f) {
        for (float j = (((int)i / 20) % 2 == 0) ? 20.0f : 0.0f; j < 600.0f; j += 40.0f) {
            rn->fill_rect(rn, &OMG_FRECT(i, j, 20, 20), &OMG_RGB(0, 128, 0));
        }
    }
    rn->set_target(rn, NULL);
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
