#include <logo.h>
#include <aamain.h>
#define base ((OMG_Scene*)this)
#define rn app->ren

bool logo_scene_on_update(LogoScene* this) {
    App* app = base->data;
    app->clock->update(app->clock);
    base->dt = app->clock->dt;
    this->logo_timer += base->dt;
    return false;
}

bool logo_scene_on_paint(LogoScene* this) {
    App* app = base->data;
    rn->begin(rn);
    rn->clear(rn, &OMG_RGB(0, 0, 0));
    float scale = (float)(this->logo_timer * this->logo_timer) / 5.0f + 0.5f;
    OMG_FRect logo_dst;
    logo_dst.x = 320.0f - this->logo->size.w * scale / 2.0f;
    logo_dst.y = 240.0f - this->logo->size.h * scale / 2.0f;
    logo_dst.w = this->logo->size.w * scale;
    logo_dst.h = this->logo->size.h * scale;
    if (scale > 1.5f) {
        if (scale > 2.5f)
            scale = 2.5f;
        float op = (2.5f - scale) * 255.0f;
        rn->tex_set_color_mod(rn, this->logo, &OMG_RGBA(255, 255, 255, op));
    }
    rn->copy_ex(rn, this->logo, NULL, &logo_dst, NULL, 0.0);
    rn->fill_rect_ex(rn, &OMG_FRECT(0, 450, 320, 20), 2.0f, &OMG_RGB(0, 255, 255));
    rn->flip(rn);
    return false;
}

bool logo_scene_on_run(LogoScene* this) {
    App* app = base->data;
    rn->set_scale(rn, NULL, &OMG_FPOINT(app->win->size.w / 640.0f, app->win->size.h / 480.0f));
    this->logo_timer = 0.0;
    loader_run(&app->ld);
    app->clock->reset(app->clock);
    return false;
}

void logo_scene_on_resize(LogoScene* this, OMG_EventResize* event) {
    App* app = base->data;
    rn->set_scale(rn, NULL, &OMG_FPOINT(event->size.w / 640.0f, event->size.h / 480.0f));
}

void logo_scene_on_keyboard(LogoScene* this, OMG_EventKeyboard* event) {
    App* app = base->data;
    if (IS_BACK_CODE(event->code)) {
        omg_scenemgr_scene_destroy(&app->sm, this);
        app->omg->auto_loop_stop(app->omg);
    }
}

bool logo_scene_on_destroy(LogoScene* this) {
    App* app = base->data;
    rn->tex_destroy(rn, this->logo);
    return false;
}

bool logo_scene_init(LogoScene* this) {
    App* app = base->data;
    this->logo = app_load_texture(app, &OMG_STR("logo.png"));
    OMG_BEGIN_POINTER_CAST();
    base->on_run = logo_scene_on_run;
    base->on_update = logo_scene_on_update;
    base->on_paint = logo_scene_on_paint;
    base->on_resize = logo_scene_on_resize;
    base->on_key_down = logo_scene_on_keyboard;
    base->on_destroy = logo_scene_on_destroy;
    OMG_END_POINTER_CAST();
    base->inited = true;
    base->was_allocated = true;
    return false;
}
