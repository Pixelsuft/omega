#include <menu.h>
#include <game.h>
#include <aamain.h>
#define base ((OMG_Scene*)this)
#define rn app->ren

bool menu_scene_on_update(MenuScene* this) {
    App* app = base->data;
    app->clock->update(app->clock);
    base->dt = app->clock->dt;
    app->au->update(app->au);
    OMG_BEGIN_POINTER_CAST();
    this->sc_t1.parent.on_update(&this->sc_t1, this);
    this->sc_t2.parent.on_update(&this->sc_t2, this);
    OMG_END_POINTER_CAST();
    return false;
}

bool menu_scene_on_paint(MenuScene* this) {
    App* app = base->data;
    rn->begin(rn);
    rn->clear(rn, &OMG_RGB(0, 0, 0));
    OMG_FRect bg_r;
    bg_r.x = -100.0f + 50.0f * (float)app->omg->std->sin(this->sc_t1.time);
    bg_r.y = -70.0f + 40.0f * (float)app->omg->std->sin((this->sc_t1.time + 0.5) * 2.0);
    bg_r.w = bg_r.h = 0.0f;
    double rot = app->omg->std->cos(this->sc_t1.time) * 0.1;
    int col_id = (int)this->sc_t2.time;
    float st = (float)(this->sc_t2.time - (double)col_id) * 255.0f;
    OMG_Color bg_col;
    bg_col.a = 255.0f;
    if (col_id == 0) {
        bg_col.r = 0.0f;
        bg_col.g = 255.0f;
        bg_col.b = st;
    }
    else if (col_id == 1) {
        bg_col.r = st;
        bg_col.g = 255.0f;
        bg_col.b = 255.0f - st;
    }
    else if (col_id == 2) {
        bg_col.r = 255.0f - st;
        bg_col.g = 255.0f;
        bg_col.b = 0.0f;
    }
    else if (col_id == 3) {
        bg_col.r = 0.0f;
        bg_col.g = 255.0f;
        bg_col.b = 0.0f;
    }
    rn->tex_set_color_mod(rn, this->bg, &bg_col);
    rn->copy_ex(rn, this->bg, NULL, &bg_r, NULL, rot);
    rn->set_scale(rn, NULL, &OMG_FPOINT(app->sc.w / 2.0f, app->sc.h / 2.0f));
    rn->tex_set_color_mod(rn, app->ld.fnt[0].page, &bg_col);
    omg_bmfont_render(&app->ld.fnt[0], &OMG_STR("TEST GAME!!!"), &OMG_FPOINT(290, 100));
    rn->tex_set_color_mod(rn, app->ld.fnt[0].page, &OMG_RGBA(255, 255, 255, 255));
    rn->set_scale(rn, NULL, &OMG_FPOINT(app->sc.w / 1.25f / 3.0f, app->sc.h / 1.25f / 3.0f));
    app_draw_fps(app);
    rn->set_scale(rn, NULL, &app->sc);
    rn->flip(rn);
    return false;
}

bool menu_scene_on_run(MenuScene* this) {
    App* app = base->data;
    rn->set_scale(rn, &OMG_FPOINT(0, 0), &OMG_FPOINT(1, 1));
    app->au->mus_play(app->au, app->ld.mus[0], -1, 0.0, 0.2);
    app->clock->reset(app->clock);
    return false;
}

void menu_scene_on_resize(MenuScene* this, OMG_EventResize* event) {
    App* app = base->data;
    app->sc.w = event->size.w / 640.0f;
    app->sc.h = event->size.h / 480.0f;
    rn->set_scale(rn, NULL, &app->sc);
}

void menu_scene_on_keyboard(MenuScene* this, OMG_EventKeyboard* event) {
    App* app = base->data;
    if ((IS_BACK_CODE(event->code) || IS_EXIT_CODE(event->code))) {
        if (!event->is_pressed) {
            omg_scenemgr_scene_destroy(&app->sm, this);
            app->omg->auto_loop_stop(app->omg);
        }
    }
    else if (event->code == OMG_SCANCODE_F && event->is_pressed) {
        app->win->set_window_mode(
            app->win,
            app->win->window_mode == OMG_WIN_MODE_WINDOW ? OMG_WIN_MODE_DESKTOP_FULLSCREEN : OMG_WIN_MODE_WINDOW
        );
    }
    else if (event->code == OMG_SCANCODE_V && event->is_pressed) {
        rn->set_vsync(rn, !app->win->vsync);
    }
    else if (event->code == OMG_SCANCODE_S && event->is_pressed) {
        app->show_fps = !app->show_fps;
    }
    else if (event->code == OMG_SCANCODE_A && event->is_pressed) {
        app->sc.w = app->sc.h = ((app->sc.w > app->sc.h) ? app->sc.h : app->sc.w);
    }
    else if (event->is_pressed) {
        this->should_cont = true;
        omg_scenemgr_scene_destroy(&app->sm, this);
    }
}

void menu_scene_on_touch(MenuScene* this, OMG_EventTouch* event) {
    App* app = base->data;
    if (event->pos.y < 0.2f) {
        if (event->pos.x < 0.2f)
            rn->set_vsync(rn, !app->win->vsync);
        else if (event->pos.x >= 0.8f)
            app->win->set_window_mode(
                app->win,
                app->win->window_mode == OMG_WIN_MODE_WINDOW ? OMG_WIN_MODE_DESKTOP_FULLSCREEN : OMG_WIN_MODE_WINDOW
            );
        else
            app->show_fps = !app->show_fps;
    }
    else {
        this->should_cont = true;
        omg_scenemgr_scene_destroy(&app->sm, this);
    }
}

bool menu_scene_on_destroy(MenuScene* this) {
    App* app = base->data;
    rn->tex_destroy(rn, this->bg);
    return false;
}

bool menu_scene_on_stop(MenuScene* this) {
    App* app = base->data;
    app->au->mus_stop(app->au, app->ld.mus[0]);
    if (!this->should_cont)
        return false;
    GameScene* scene = OMG_MALLOC(app->omg->mem, sizeof(GameScene));
    omg_scenemgr_scene_fill(&app->sm, scene);
    OMG_BEGIN_POINTER_CAST();
    scene->parent.on_init = game_scene_init;
    OMG_END_POINTER_CAST();
    omg_scenemgr_scene_init(&app->sm, scene, app);
    omg_scenemgr_scene_run(&app->sm, scene);
    return false;
}

bool menu_scene_init(MenuScene* this) {
    App* app = base->data;
    app->sc.w = app->win->size.w / 640.0f;
    app->sc.h = app->win->size.h / 480.0f;
    rn->set_scale(rn, &OMG_FPOINT(0, 0), &OMG_FPOINT(1, 1));
    rn->tex_set_color_mod(rn, app->ld.fnt[0].page, &OMG_RGB(255, 255, 255));
    rn->tex_set_scale_mode(rn, app->ld.fnt[0].page, OMG_SCALE_MODE_LINEAR);
    this->bg = rn->tex_create(rn, NULL, &OMG_FPOINT(800, 600), OMG_TEXTURE_ACCESS_TARGET, false);
    rn->set_target(rn, this->bg);
    rn->clear(rn, &OMG_RGB(0, 0, 0));
    for (float i = 0.0f; i < 800.0f; i += 20.0f) {
        for (float j = (((int)i / 20) % 2 == 0) ? 20.0f : 0.0f; j < 600.0f; j += 40.0f) {
            rn->fill_rect(rn, &OMG_FRECT(i, j, 20, 20), &OMG_RGB(128, 128, 128));
        }
    }
    rn->set_scale(rn, NULL, &app->sc);
    rn->set_target(rn, NULL);
    omg_obj_anim_timer_init(&this->sc_t1, app->omg);
    omg_obj_anim_timer_init(&this->sc_t2, app->omg);
    this->sc_t1.running = this->sc_t2.running = true;
    this->sc_t1.duration = OMG_M_PI2;
    this->sc_t2.duration = 4.0;
    app->au->mus_set_volume(app->au, app->ld.mus[0], 0.1f);
    base->reset_input = false;
    OMG_BEGIN_POINTER_CAST();
    base->on_run = menu_scene_on_run;
    base->on_update = menu_scene_on_update;
    base->on_paint = menu_scene_on_paint;
    base->on_resize = menu_scene_on_resize;
    base->on_key_down = menu_scene_on_keyboard;
    base->on_key_up = menu_scene_on_keyboard;
    base->on_touch_up = menu_scene_on_touch;
    base->on_destroy = menu_scene_on_destroy;
    base->on_stop = menu_scene_on_stop;
    OMG_END_POINTER_CAST();
    this->should_cont = false;
    base->inited = true;
    base->was_allocated = true;
    return false;
}
