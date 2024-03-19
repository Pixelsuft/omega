#include <logo.h>
#include <aamain.h>
#define base ((OMG_Scene*)this)
#define rn app->ren

bool logo_scene_on_update(LogoScene* this) {
    OMG_UNUSED(this);
    return false;
}

bool logo_scene_on_paint(LogoScene* this) {
    App* app = base->data;
    rn->begin(rn);
    rn->clear(rn, &OMG_RGB(0, 0, 0));
    rn->fill_rect_ex(rn, &OMG_FRECT(0, 450, 320, 20), 2.0f, &OMG_RGB(0, 255, 255));
    rn->flip(rn);
    return false;
}

bool logo_scene_on_run(LogoScene* this) {
    App* app = base->data;
    rn->set_scale(rn, NULL, &OMG_FPOINT(app->win->size.w / 640.0f, app->win->size.h / 480.0f));
    app->clock->reset(app->clock);
    OMG_INFO(app->omg, "Scene run!");
    return false;
}

void logo_scene_on_resize(LogoScene* this, OMG_EventResize* event) {
    App* app = base->data;
    rn->set_scale(rn, NULL, &OMG_FPOINT(event->size.w / 640.0f, event->size.h / 480.0f));
}

void logo_scene_on_keyboard(LogoScene* this, OMG_EventKeyboard* event) {
    App* app = base->data;
    if (IS_BACK_CODE(event->code)) {
        OMG_INFO(app->omg, "Exit!!!");
        omg_scenemgr_scene_destroy(&app->sm, this);
    }
}

bool logo_scene_on_destroy(LogoScene* this) {
    printf("test test");
    return false;
}

bool logo_scene_init(LogoScene* this) {
    App* app = base->data;
    OMG_INFO(app->omg, "Scene init!");
    OMG_BEGIN_POINTER_CAST();
    base->on_run = logo_scene_on_run;
    base->on_update = logo_scene_on_update;
    base->on_paint = logo_scene_on_paint;
    base->on_resize = logo_scene_on_resize;
    base->on_key_down = logo_scene_on_keyboard;
    base->on_destroy = logo_scene_on_destroy;
    OMG_END_POINTER_CAST();
    return false;
}
