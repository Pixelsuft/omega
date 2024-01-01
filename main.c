#include <omega/omega.h>
#include <omega/entry.h>
#include <omega/clock.h>
#include <omega/omega_win.h>
#include <omega/omega_sdl2.h>
#include <omega/omega_raylib.h>
#include <omega/window_win.h>

typedef struct {
    OMG_Omega* omg;
    OMG_Window* win;
    OMG_Renderer* ren;
    OMG_Clock* clock;
    omg_color_t bg_col;
    bool bg_fow;
    int exit_code;
} App;

int omega_main(OMG_EntryData* data);
OMG_MAKE_MAIN(omega_main)

void app_on_destroy(OMG_EventLoopStop* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    // everything other will be cleaned up automaticly
    this->ren->destroy(this->ren);
    this->omg->app_quit(this->omg);
    OMG_INFO(
        this->omg,
        "Exit. Number of allocations: ",
        (int)this->omg->mem->get_alloc_count(this->omg->mem)
    );
    this->omg->destroy(this->omg);
    this->exit_code = 0;
}

void app_on_state_changing(OMG_EventStateChanging* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    if (event->change == OMG_WIN_STATE_CLOSED) {
        OMG_UNUSED(this);
        // You can prevent window from being closed
        // event->allow = false;
        return;
    }
}

void app_on_size_change(OMG_EventResize* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    OMG_INFO(this->omg, "Resize: [", &event->size, "]");
}

void app_on_mouse_move(OMG_EventMouseMove* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    OMG_UNUSED(this);
    // OMG_INFO(this->omg, "Mouse Move [", &event->pos, "]");
}

void app_on_mouse_button(OMG_EventMouseButton* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    OMG_INFO(
        this->omg,
        "Mouse Button ", event->is_pressed ? "Press" : "Release" ," [", &event->pos, "], ",
        (int)event->clicks
    );
}

void app_on_keyboard(OMG_EventKeyboard* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    // TODO: print bool
    OMG_INFO(
        this->omg,
        "Key ",
        omg_keyboard_name_from_scancode(event->scancode)->ptr,
        " (",
        omg_keyboard_name_from_key(event->sym)->ptr,
        ") ",
        event->is_pressed ? "Press" : "Release"
    );
}

void app_on_update(OMG_EventUpdate* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    if (this->clock->update(this->clock)) {
        // FPS limit, so let's check events again
        this->omg->enable_paint = false;
        return;
    }
    this->omg->enable_paint = true;
    if (this->bg_fow) {
        this->bg_col += this->clock->dt * 100.0f;
        if (this->bg_col >= OMG_MAX_COLOR) {
            this->bg_col = OMG_MAX_COLOR * (omg_color_t)2 - this->bg_col;
            this->bg_fow = false;
        }
    }
    else {
        this->bg_col -= this->clock->dt * 100.0f;
        if (this->bg_col < (omg_color_t)0) {
            this->bg_col = -this->bg_col;
            this->bg_fow = true;
        }
    }
    // OMG_INFO(this->omg, "FPS: ", this->clock->get_fps(this->clock));
    // OMG_INFO(this->omg, "DT: ", this->clock->dt);
}

void app_on_paint(OMG_EventPaint* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    // You can use something like this to detect window, but we have only one
    // if (event->win != this->win)
    //     return;
    this->ren->begin(this->ren);
    this->ren->clear(this->ren, &OMG_COLOR_MAKE_RGB(this->bg_col, this->bg_col, this->bg_col));
    this->ren->flip(this->ren);
}

void app_init(App* this, OMG_EntryData* data) {
    this->exit_code = 1;
#if OMG_SUPPORT_WIN
    this->omg = (OMG_Omega*)omg_win_create(data);
#endif
#if OMG_SUPPORT_SDL2
    this->omg = (OMG_Omega*)omg_sdl2_create(data);
#endif
#if OMG_SUPPORT_RAYLIB
    this->omg = (OMG_Omega*)omg_raylib_create(data);
#endif
    if (OMG_ISNULL(this->omg) || this->omg->omg_init(this->omg)) {
        return;
    }
    if (
        this->omg->app_init(this->omg) ||
        this->omg->winmgr_alloc(this->omg) ||
        this->omg->winmgr->init(this->omg->winmgr) ||
        (OMG_ISNULL(this->win = this->omg->winmgr->window_alloc(this->omg->winmgr)))
    ) {
        OMG_ERROR(this->omg, "OMG Init Fail");
        this->omg->destroy(this->omg);
        return;
    }
    this->win->resizable = false;
    if (this->win->default_init(this->win)) {
        OMG_ERROR(this->omg, "OMG Window Init Fail");
        this->omg->destroy(this->omg);
        return;
    }
    if (this->win->renderer_alloc(this->win) || this->win->ren->init(this->win->ren)) {
        OMG_ERROR(this->omg, "OMG Renderer Init fail");
        this->omg->destroy(this->omg);
        return;
    }
    this->clock = this->omg->clock;
    this->ren = this->win->ren;
    this->omg->event_arg = this;
    this->omg->on_update = app_on_update;
    this->omg->on_paint = app_on_paint;
    this->omg->on_loop_stop = app_on_destroy;
    this->omg->on_size_change = app_on_size_change;
    this->omg->on_state_changing = app_on_state_changing;
    this->omg->on_mouse_move = app_on_mouse_move;
    this->omg->on_mouse_down = this->omg->on_mouse_up = app_on_mouse_button;
    this->omg->on_key_down = this->omg->on_key_up = app_on_keyboard;
    this->bg_col = (omg_color_t)0;
    this->bg_fow = true;
    this->clock->init(this->clock, true);
    this->clock->wait_for_limit = false;
    this->win->set_title(this->win, &OMG_STRING_MAKE_STATIC("Test Window"));
    OMG_INFO(this->omg, 1337.228f, L" win32 is shit btw ", 228.1337, " 1", 228, "1 0x", (void*)this->omg);
    // this->clock->set_fps_limit(this->clock, 5.0);
    this->clock->reset(this->clock);
    this->win->show(this->win, true);
    this->exit_code = 0;
}

int omega_main(OMG_EntryData* data) {
    App app;
    app_init(&app, data);
    if (app.exit_code)
        return app.exit_code;
    app.omg->auto_loop_run(app.omg);
    return app.exit_code;
}
