#include <omega/omega.h>
#include <omega/entry.h>
#include <omega/omega_win.h>
#include <omega/omega_sdl2.h>
#include <omega/omega_raylib.h>

typedef struct {
    OMG_Omega* omg;
    OMG_Window* win;
    OMG_Renderer* ren;
    OMG_Surface* surf;
    OMG_Texture* tex;
    OMG_Texture* tex2;
    OMG_File* file;
    OMG_Clock* clock;
    omg_color_t bg_col;
    bool bg_fow;
    int exit_code;
} App;

int omega_main(OMG_EntryData* data);
OMG_MAIN_MAKE(omega_main)

void app_on_destroy(OMG_EventLoopStop* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    this->ren->tex_destroy(this->ren, this->tex2);
    this->ren->tex_destroy(this->ren, this->tex);
    this->omg->winmgr->surf_destroy(this->omg->winmgr, this->surf);
    this->file->destroy(this->file);
    // everything other will be cleaned up automaticly
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

void app_on_mouse_move(OMG_EventMouseMove* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    if (event->state & OMG_MBUTTON_LMASK) {
        this->ren->set_scale(this->ren, &OMG_FPOINT_MAKE(
            this->ren->offset.x + event->rel.x / this->ren->scale.x,
            this->ren->offset.y + event->rel.y / this->ren->scale.y
        ), NULL);        
    }
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
    if (event->is_pressed) {
        if (event->code == OMG_SCANCODE_Q) {
            // Quit when Q is pressed
            this->omg->auto_loop_stop(this->omg);
            return;
        }
        else if (event->code == OMG_SCANCODE_R)
            this->ren->set_scale(this->ren, &OMG_FPOINT_MAKE(0.0f, 0.0f), &OMG_FPOINT_MAKE(1.0f, 1.0f));
        else if (event->code == OMG_SCANCODE_UP)
            this->ren->set_scale(this->ren, NULL, &OMG_FPOINT_MAKE(this->ren->scale.x, this->ren->scale.y + 0.25f));
        else if (event->code == OMG_SCANCODE_DOWN)
            this->ren->set_scale(this->ren, NULL, &OMG_FPOINT_MAKE(this->ren->scale.x, this->ren->scale.y - 0.25f));
        else if (event->code == OMG_SCANCODE_RIGHT)
            this->ren->set_scale(this->ren, NULL, &OMG_FPOINT_MAKE(this->ren->scale.x + 0.25f, this->ren->scale.y));
        else if (event->code == OMG_SCANCODE_LEFT)
            this->ren->set_scale(this->ren, NULL, &OMG_FPOINT_MAKE(this->ren->scale.x - 0.25f, this->ren->scale.y));
    }
    // TODO: print bool
    if (!event->is_repeated)
        OMG_INFO(
            this->omg,
            "Key ",
            omg_keyboard_name_from_scancode(event->code)->ptr,
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
            this->bg_col = OMG_MAX_COLOR * 2.0f - this->bg_col;
            this->bg_fow = false;
        }
    }
    else {
        this->bg_col -= this->clock->dt * 100.0f;
        if (this->bg_col < 0.0f) {
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
    OMG_FPoint pos;
    for (pos.x = 100.0f; pos.x < 150.0f; pos.x += 2.0f) {
        for (pos.y = 100.0f; pos.y < 150.0f; pos.y += 2.0f) {
            this->ren->draw_point(this->ren, &pos, &OMG_COLOR_MAKE_RGB(0, 255, 0));
        }
    }
    this->ren->fill_rect(this->ren, &OMG_FRECT_MAKE(200, 200, 50, 50), &OMG_COLOR_MAKE_RGBA(255, 0, 0, 100));
    this->ren->draw_line(this->ren, &OMG_FRECT_MAKE(0, 100, 200, 200), &OMG_COLOR_MAKE_RGB(0, 0, 255));
    this->ren->fill_circle(this->ren, &OMG_FPOINT_MAKE(300, 300), 50.0f, &OMG_COLOR_MAKE_RGBA(0, 255, 255, 128));
    this->ren->set_target(this->ren, this->tex);
    // Drawing on a texture
    this->ren->clear(this->ren, &OMG_COLOR_MAKE_RGBA(0, 255, 255, 100));
    this->ren->fill_rect(this->ren, &OMG_FRECT_MAKE(50, 50, 100, 100), &OMG_COLOR_MAKE_RGBA(255, 0, 0, 255));
    this->ren->set_target(this->ren, NULL);
    this->ren->copy(this->ren, this->tex, &OMG_FPOINT_MAKE(400, 200));
    this->ren->copy(this->ren, this->tex2, &OMG_FPOINT_MAKE(200, 400));
    this->ren->flip(this->ren);
    // printf("%i\n", this->clock->get_fps(this->clock));
    // this->omg->delay(this->omg, 1.0 / 60.0);
}

void app_on_expose(OMG_EventExpose* event) {
    // Be careful with pointer casting
    app_on_update((OMG_EventUpdate*)event);
    app_on_paint((OMG_EventPaint*)event);
}

void app_on_size_change(OMG_EventResize* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    OMG_INFO(this->omg, "Resize: [", &event->size, "]");
}

void app_init(App* this, OMG_EntryData* data) {
    this->exit_code = 1;
#if OMG_SUPPORT_RAYLIB
    this->omg = (OMG_Omega*)omg_raylib_create(data);
#endif
#if OMG_SUPPORT_WIN
    this->omg = (OMG_Omega*)omg_win_create(data);
#endif
#if OMG_SUPPORT_SDL2
    this->omg = (OMG_Omega*)omg_sdl2_create(data);
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
    this->win->resizable = true;
    if (this->win->default_init(this->win)) {
        OMG_ERROR(this->omg, "OMG Window Init Fail");
        this->omg->destroy(this->omg);
        return;
    }
    if (
        this->omg->winmgr->image_loader_alloc(this->omg->winmgr) ||
        this->omg->winmgr->img->init(this->omg->winmgr->img)
    ) {
        OMG_ERROR(this->omg, "OMG Image Loader Init Fail");
        this->omg->destroy(this->omg);
        return;
    }
    if (this->win->type == OMG_WIN_TYPE_WIN && 0)
        this->win->ren_type = OMG_REN_TYPE_WIN;
    if (this->win->renderer_alloc(this->win) || this->win->ren->init(this->win->ren)) {
        OMG_ERROR(this->omg, "OMG Renderer Init Fail");
        this->omg->destroy(this->omg);
        return;
    }
    this->ren = this->win->ren;
    this->ren->soft_scale = true;
    this->ren->aa = true;
    this->file = this->omg->file_from_path(this->omg, NULL, &OMG_STRING_MAKE_STATIC("assets/sample.txt"), OMG_FILE_MODE_RT);
    if (OMG_ISNULL(this->file)) {
        OMG_ERROR(this->omg, "OMG File Open Fail");
        this->omg->destroy(this->omg);
        return;
    }
    // I'm lazy for fail checks here, but you shouldn't :)
    int64_t file_size = this->file->get_size(this->file);
    OMG_INFO(this->omg, "File size: ", (int)file_size);
    OMG_String file_buf;
    omg_string_init_dynamic(&file_buf, NULL);
    if (file_size > 0)
        omg_string_resize(&file_buf, (size_t)file_size);
    this->file->read(this->file, file_buf.ptr, 1, file_size);
    OMG_INFO(this->omg, "File content: ", &file_buf);
    OMG_INFO(this->omg, "File pos: ", (int)this->file->tell(this->file));
    omg_string_destroy(&file_buf);
    this->clock = this->omg->clock;
    this->win->allow_alt = false;
    this->omg->event_arg = this;
    this->omg->on_update = app_on_update;
    this->omg->on_paint = app_on_paint;
    this->omg->on_expose = app_on_expose;
    this->omg->on_loop_stop = app_on_destroy;
    this->omg->on_size_change = app_on_size_change;
    this->omg->on_state_changing = app_on_state_changing;
    this->omg->on_mouse_move = app_on_mouse_move;
    this->omg->on_mouse_down = this->omg->on_mouse_up = app_on_mouse_button;
    this->omg->on_key_down = this->omg->on_key_up = app_on_keyboard;
    this->bg_col = 0.0f;
    this->bg_fow = true;
    this->win->set_min_size(this->win, &OMG_FPOINT_MAKE(320, 200));
    this->surf = this->omg->winmgr->surf_create(this->omg->winmgr, &OMG_FPOINT_MAKE(200, 200), true);
    this->tex = this->ren->tex_create(this->ren, &OMG_FPOINT_MAKE(200, 200), OMG_TEXTURE_ACCESS_TARGET, true);
    this->tex2 = this->ren->tex_from_surf(this->ren, 
        this->omg->winmgr->surf_from_path(this->omg->winmgr, &OMG_STRING_MAKE_STATIC("assets/sprite.png")),
    true);
    this->clock->init(this->clock, true);
    this->clock->wait_for_limit = false;
    this->win->set_title(this->win, &OMG_STRING_MAKE_STATIC("Test Window"));
    OMG_INFO(this->omg, 1337.228f, " ", 228.1337, " 1", 228, "1 0x", (void*)this->omg);
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
