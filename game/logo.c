#include <logo.h>
#include <aamain.h>
#include <menu.h>
#define base ((OMG_Scene*)this)
#define rn app->ren

// Update every frame before draw
bool logo_scene_on_update(LogoScene* this) {
    App* app = base->data;
    app->clock->update(app->clock);
    base->dt = app->clock->dt; // Time in seconds since last update
    // Update files loader
    loader_update(&app->ld);
    this->logo_timer += base->dt; // For simple animation
    if (app->ld.finished && OMG_DEBUG && !this->should_cont && 0) { // Debug
        this->should_cont = true;
        omg_scenemgr_scene_destroy(&app->sm, this);
    }
    return false;
}

// Paint after update
bool logo_scene_on_paint(LogoScene* this) {
    App* app = base->data;
    // Begin drawing
    rn->begin(rn);
    // Fill screen with black color
    rn->clear(rn, &OMG_RGB(0, 0, 0));
    // Logo texture scale anim
    float scale = (float)(this->logo_timer * this->logo_timer) / 5.0f + 0.5f;
    OMG_FRect logo_dst;
    logo_dst.x = 320.0f - this->logo->size.w * scale / 2.0f;
    logo_dst.y = 240.0f - this->logo->size.h * scale / 2.0f;
    logo_dst.w = this->logo->size.w * scale;
    logo_dst.h = this->logo->size.h * scale;
    if (scale > 1.5f) {
        // Logo texture transparency anim
        if (scale > 2.5f)
            scale = 2.5f;
        float op = (2.5f - scale) * 255.0f;
        rn->tex_set_color_mod(rn, this->logo, &OMG_RGBA(op, op, op, 255));
        rn->tex_set_blend_mode(rn, app->ld.fnt[0].page, OMG_BLEND_MODE_BLEND);
        rn->tex_set_color_mod(rn, app->ld.fnt[0].page, &OMG_RGBA(255 - op, 255 - op, 255 - op, 255 - op));
    }
    // Display progress
    float prog = (float)app->ld.progress / (float)app->ld.total_count;
    rn->copy_ex(rn, this->logo, NULL, &logo_dst, NULL, 0.0);
    rn->fill_rect_ex(rn, &OMG_FRECT(0, 450, prog * 640, 20), 4.0f, &OMG_RGB(0, 255, 255));
    if ((scale > 1.5f) && app->ld.finished) {
        // Draw bmfont text
        rn->tex_set_scale_mode(rn, app->ld.fnt[0].page, OMG_SCALE_MODE_LINEAR);
        float adv_scale = 0.5f + (float)app->omg->std->sin(this->logo_timer) / 100.0f;
        rn->set_scale(rn, NULL, &OMG_FPOINT(app->sc.w * adv_scale, app->sc.h * adv_scale));
        omg_bmfont_render(&app->ld.fnt[0], &OMG_STR("HELLO, FMS SFU! ;)"), &OMG_FPOINT(150, 700));
        rn->set_scale(rn, NULL, &app->sc);
    }
    // Show everything we just drawed
    rn->flip(rn);
    return false;
}

bool logo_scene_on_run(LogoScene* this) {
    // Scene start event
    App* app = base->data;
    // Scale magic to make app working with different window sizes
    app->sc.w = app->ren->size.w / 640.0f;
    app->sc.h = app->ren->size.h / 480.0f;
    rn->set_scale(rn, NULL, &app->sc);
    this->logo_timer = 0.0;
    loader_run(&app->ld);
    app->clock->reset(app->clock);
    return false;
}

void logo_scene_on_resize(LogoScene* this, OMG_EventResize* event) {
    // Scene resize event
    App* app = base->data;
    app->sc.w = event->size.w / 640.0f;
    app->sc.h = event->size.h / 480.0f;
    rn->set_scale(rn, NULL, &app->sc);
}

void logo_scene_on_keyboard(LogoScene* this, OMG_EventKeyboard* event) {
    App* app = base->data;
    // Allow to skip logo only if all files are loaded
    if (app->ld.finished) {
        // Should we quit (ESC or Q pressed)?
        this->should_cont = !(IS_BACK_CODE(event->code) || IS_EXIT_CODE(event->code));
        bool sc = this->should_cont; // Create var because scene will be destroyed
        omg_scenemgr_scene_destroy(&app->sm, this);
        if (!sc)
            app->omg->auto_loop_stop(app->omg);
    }
}

void logo_scene_on_touch(LogoScene* this, OMG_EventTouch* event) {
    App* app = base->data;
    OMG_UNUSED(event);
    if (app->ld.finished) {
        // Same for touch
        this->should_cont = true;
        omg_scenemgr_scene_destroy(&app->sm, this);
    }
}

bool logo_scene_on_destroy(LogoScene* this) {
    App* app = base->data;
    // Cleanup
    rn->tex_destroy(rn, this->logo);
    return false;
}

bool logo_scene_on_stop(LogoScene* this) {
    App* app = base->data;
    if (!this->should_cont)
        return false;
    // Stop event, so let's run Menu scene if we need
    MenuScene* scene = OMG_MALLOC(app->omg->mem, sizeof(MenuScene));
    omg_scenemgr_scene_fill(&app->sm, scene);
    OMG_BEGIN_POINTER_CAST();
    scene->parent.on_init = menu_scene_init;
    OMG_END_POINTER_CAST();
    omg_scenemgr_scene_init(&app->sm, scene, app);
    omg_scenemgr_scene_run(&app->sm, scene);
    return false;
}

bool logo_scene_init(LogoScene* this) {
    App* app = base->data;
    // Init our class
    this->should_cont = false;
    this->logo = app_load_texture(app, &OMG_STR("logo.png"));
    OMG_Surface* icon = app_load_surf(app, &OMG_STR("icon.png"));
    app->win->set_icon(app->win, icon);
    app->omg->winmgr->surf_destroy(app->omg->winmgr, icon);
    rn->tex_set_scale_mode(rn, this->logo, OMG_SCALE_MODE_LINEAR);
    app->sc.w = app->ren->size.w / 640.0f;
    app->sc.h = app->ren->size.h / 480.0f;
    base->reset_input = false;
    OMG_BEGIN_POINTER_CAST();
    base->on_run = logo_scene_on_run;
    base->on_update = logo_scene_on_update;
    base->on_paint = logo_scene_on_paint;
    base->on_resize = logo_scene_on_resize;
    base->on_key_down = logo_scene_on_keyboard;
    base->on_destroy = logo_scene_on_destroy;
    base->on_stop = logo_scene_on_stop;
    base->on_touch_up = logo_scene_on_touch;
    OMG_END_POINTER_CAST();
    base->inited = true;
    base->was_allocated = true;
    return false;
}
