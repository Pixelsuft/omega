#include <omega/omega.h>
#include <omega/entry.h>
#include <omega/omega_win.h>
#include <omega/omega_sdl2.h>
#include <omega/omega_raylib.h>

typedef struct {
    OMG_Omega* omg;
    OMG_Window* win;
    OMG_Renderer* ren;
    OMG_Audio* audio;
    OMG_Surface* surf;
    OMG_Texture* tex;
    OMG_Texture* tex2;
    OMG_Music* mus;
    OMG_Sound* sound;
    OMG_File* file;
    OMG_Clock* clock;
    OMG_FontMgr* fnt;
    OMG_Font* fps_font;
    OMG_FPoint scale_cache;
    OMG_FPoint offset_cache;
    OMG_String fps_str;
    char fps_buf[20];
    double sin_mul;
    double rot_timer;
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
    this->audio->snd_destroy(this->audio, this->sound);
    this->audio->mus_destroy(this->audio, this->mus);
    this->fnt->font_destroy(this->fnt, this->fps_font);
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
        this->offset_cache.x = this->offset_cache.x + event->rel.x / this->ren->scale.x;
        this->offset_cache.y = this->offset_cache.y + event->rel.y / this->ren->scale.y;
        this->ren->set_scale(this->ren, &this->offset_cache, NULL);        
    }
}

void app_on_mouse_button(OMG_EventMouseButton* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    OMG_INFO(
        this->omg,
        "Mouse Button ", event->is_pressed ? "Press" : "Release" ," [", &event->pos, "], ",
        (int)event->clicks
    );
    if (event->button == OMG_MBUTTON_RIGHT && event->is_pressed){
        this->audio->snd_play(this->audio, this->sound, 0, 0.0);
        return;
    }
}

void app_on_keyboard(OMG_EventKeyboard* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    if (event->is_pressed) {
        if (event->code == OMG_SCANCODE_Q) {
            // Quit when Q is pressed
            this->omg->auto_loop_stop(this->omg);
            return;
        }
        else if (event->code == OMG_SCANCODE_R) {
            this->scale_cache.x = this->scale_cache.y = 1.0f;
            this->fnt->font_set_scale(this->fnt, this->fps_font, &OMG_FPOINT_MAKE(1, 1));
            this->ren->set_scale(this->ren, &OMG_FPOINT_MAKE(0, 0), &OMG_FPOINT_MAKE(1, 1));
        }
        else if (event->code == OMG_SCANCODE_UP) {
            this->scale_cache.y += 0.25f;
            this->fnt->font_set_scale(this->fnt, this->fps_font, &this->scale_cache);
        }
        else if (event->code == OMG_SCANCODE_DOWN) {
            this->scale_cache.y -= 0.25f;
            this->fnt->font_set_scale(this->fnt, this->fps_font, &this->scale_cache);
        }
        else if (event->code == OMG_SCANCODE_RIGHT) {
            this->scale_cache.x += 0.25f;
            this->fnt->font_set_scale(this->fnt, this->fps_font, &this->scale_cache);
        }
        else if (event->code == OMG_SCANCODE_LEFT) {
            this->scale_cache.x -= 0.25f;
            this->fnt->font_set_scale(this->fnt, this->fps_font, &this->scale_cache);
        }
        else if (event->code == OMG_SCANCODE_A)
            this->audio->mus_play(this->audio, this->mus, -1, 0.0, 0.0);
        else if (event->code == OMG_SCANCODE_S)
            this->audio->mus_stop(this->audio, this->mus);
        else if (event->code == OMG_SCANCODE_Z || event->code == OMG_SCANCODE_X)
            this->audio->mus_set_pos(this->audio, this->mus, this->audio->mus_get_pos(this->audio, this->mus) + (event->code == OMG_SCANCODE_X ? 5.0 : -5.0));
        else if (event->code == OMG_SCANCODE_V)
            this->ren->set_vsync(this->ren, !this->win->vsync);
        else if (event->code == OMG_SCANCODE_E)
            this->ren->aa = !this->ren->aa;
        else if (event->code == OMG_SCANCODE_T)
            this->fps_font->aa = !this->fps_font->aa;
    }
    if (event->code == OMG_SCANCODE_F) {
        if (event->is_pressed)
            this->ren->tex_set_color_mod(this->ren, this->tex2, &OMG_COLOR_MAKE_RGBA(0, 255, 255, 100));
        else
            this->ren->tex_set_color_mod(this->ren, this->tex2, &OMG_COLOR_MAKE_RGBA(255, 255, 255, 255));
    }
    else if (event->code == OMG_SCANCODE_G) {
        this->fps_font->aa = !event->is_pressed;
        this->ren->tex_set_scale_mode(this->ren, this->tex2, event->is_pressed ? OMG_SCALE_MODE_LINEAR : OMG_SCALE_MODE_NEAREST);
    }
    else if (event->code == OMG_SCANCODE_H) {
        if (event->is_pressed) {
            this->ren->auto_blend = false;
            this->ren->set_blend_mode(this->ren, OMG_BLEND_MODE_MUL);
        }
        else {
            this->ren->auto_blend = true;
            this->ren->set_blend_mode(this->ren, OMG_BLEND_MODE_BLEND);
        }
    }
    else if (event->code == OMG_SCANCODE_J) {
        this->clock->speed = event->is_pressed ? 2.0 : 1.0;
        this->audio->mus_set_speed(this->audio, this->mus, (float)this->clock->speed);
    }
    else if (event->code == OMG_SCANCODE_K) {
        this->clock->speed = event->is_pressed ? 0.5 : 1.0;
        this->audio->mus_set_speed(this->audio, this->mus, (float)this->clock->speed);
    }
    else if (event->code == OMG_SCANCODE_C)
        this->sin_mul = event->is_pressed ? 8.0 : 0.4;
    if (!event->is_repeated)
        OMG_INFO(
            this->omg,
            "Key ",
            omg_keyboard_name_from_scancode(event->code)->ptr,
            " (",
            omg_keyboard_name_from_key(event->sym)->ptr,
            "). Press: ",
            event->is_pressed
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
    this->audio->update(this->audio);
    if (this->bg_fow) {
        this->bg_col += (float)this->clock->dt * 100.0f;
        if (this->bg_col >= OMG_MAX_COLOR) {
            this->bg_col = OMG_MAX_COLOR * 2.0f - this->bg_col;
            this->bg_fow = false;
        }
    }
    else {
        this->bg_col -= (float)this->clock->dt * 100.0f;
        if (this->bg_col < 0.0f) {
            this->bg_col = -this->bg_col;
            this->bg_fow = true;
        }
    }
    this->rot_timer += this->clock->dt;
    if (this->rot_timer >= OMG_M_PI2)
        this->rot_timer -= OMG_M_PI2;
    // Hack
    this->fps_str.len = 5;
    omg_string_add_int(&this->fps_str, this->clock->get_fps(this->clock));
    // OMG_INFO(this->omg, &this->fps_str);
    // OMG_INFO(this->omg, this->omg->mem->get_alloc_count(this->omg->mem));
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
    this->ren->set_scale(this->ren, &this->offset_cache, &this->scale_cache);
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
    // this->ren->fill_circle(this->ren, &OMG_FPOINT_MAKE(50, 50), 50.0f, &OMG_COLOR_MAKE_RGBA(255, 255, 0, 100));
    this->ren->set_target(this->ren, NULL);
    this->ren->copy(this->ren, this->tex, &OMG_FPOINT_MAKE(400, 200));
    this->ren->copy_ex(
        this->ren, this->tex2,
        NULL, &OMG_FRECT_MAKE(200, 400, 0, 0),
        NULL, this->omg->std->sin(this->rot_timer) * this->sin_mul
    );
    double mus_pos = this->audio->mus_get_pos(this->audio, this->mus);
    if (mus_pos > 0.0) {
        // We hardcode music len 130 sec because sometimes you can't get length with smth like this->mus->duration
        float line_w = (float)(mus_pos * 500.0 / 130.820999);
        this->ren->fill_rect(this->ren, &OMG_FRECT_MAKE(200, 500, line_w, 20), &OMG_COLOR_MAKE_RGBA(255, 0, 0, 100));
    }
    this->ren->draw_rect(this->ren, &OMG_FRECT_MAKE(200, 500, 500, 20), &OMG_COLOR_MAKE_RGB(0, 0, 255));
    // Scale font manually to avoid pixelating
    this->ren->set_scale(this->ren, &OMG_FPOINT_MAKE(this->offset_cache.x * this->scale_cache.x, this->offset_cache.y * this->scale_cache.y), &OMG_FPOINT_MAKE(1, 1));
    OMG_Texture* fps_tex = this->ren->font_render(this->ren, NULL, this->fps_font, &this->fps_str, NULL, &OMG_COLOR_MAKE_RGB(0, 255, 255), NULL);
    this->ren->copy(this->ren, fps_tex, NULL);
    this->ren->flip(this->ren);
    this->ren->tex_destroy(this->ren, fps_tex);
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
    this->win->resizable = true;
#if OMG_IS_EMSCRIPTEN
    this->win->size.w = 1152.0f;
    this->win->size.h = 864.0f;
#endif
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
    if ((!OMG_IS_EMSCRIPTEN && !OMG_IS_ANDROID/* && 0*/) || this->omg->type == OMG_OMEGA_TYPE_WIN)
        this->omg->audio_type = OMG_AUDIO_TYPE_FMOD;
    if (this->omg->audio_alloc(this->omg) || this->omg->audio->init(this->omg->audio)) {
        OMG_ERROR(this->omg, "OMG Audio Init Fail");
        this->omg->destroy(this->omg);
        return;
    }
    if (this->omg->winmgr->fontmgr_alloc(this->omg->winmgr) || this->omg->winmgr->fnt->init(this->omg->winmgr->fnt)) {
        OMG_ERROR(this->omg, "OMG Font Init Fail");
        this->omg->destroy(this->omg);
        return;        
    }
    this->fnt = this->omg->winmgr->fnt;
    this->audio = this->omg->audio;
    this->ren = this->win->ren;
    this->offset_cache.x = this->offset_cache.y = 0.0f;
    this->scale_cache.x = this->scale_cache.y = 1.0f;
    this->ren->aa = !OMG_IS_EMSCRIPTEN; // NOTE: Someties it's pretty slow (for example, SDL2)
    // I'm lazy for fail checks here, but you shouldn't :)
    this->fps_font = this->fnt->font_from_fp(this->fnt, NULL, &OMG_STRING_MAKE_STATIC("assets/segoeuib.ttf"), -1, 32.0f);
    this->mus = this->audio->mus_from_fp(this->audio, NULL, &OMG_STRING_MAKE_STATIC("assets/music.mp3"));
    this->sound = this->audio->snd_from_fp(this->audio, NULL, &OMG_STRING_MAKE_STATIC("assets/sound.ogg"));
    this->file = this->omg->file_from_fp(this->omg, NULL, &OMG_STRING_MAKE_STATIC("assets/sample.txt"), OMG_FILE_MODE_RT);
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
    this->file->destroy(this->file);
    this->omg->std->memcpy(this->fps_buf, "FPS:               \0", 20);
    this->fps_str = OMG_STRING_MAKE_BUFFER_A(this->fps_buf);
    this->clock = this->omg->clock;
    this->fps_font->text_type = OMG_FONT_TEXT_TYPE_TEXT; // We don't need UTF-8 support for drawing FPS
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
    this->rot_timer = 0.0;
    this->bg_col = 0.0f;
    this->bg_fow = true;
    this->win->set_min_size(this->win, &OMG_FPOINT_MAKE(320, 200));
    this->surf = this->omg->winmgr->surf_create(this->omg->winmgr, NULL, &OMG_FPOINT_MAKE(256, 256), true);
    this->omg->winmgr->surf_set_locked(this->omg->winmgr, this->surf, true);
    // I don't recommend to use surfaces because they may have different formats and you currently can't see it
    if (OMG_ISNOTNULL(this->surf->data))
        for (uint64_t* i = this->surf->data; (size_t)i - (size_t)this->surf->data < (size_t)this->surf->size.w * (size_t)this->surf->size.h * 4; i++) {
            *i = 0x000000FF | (uint64_t)((double)((size_t)i - (size_t)this->surf->data) * (double)0x4000);
        }
    this->omg->winmgr->surf_set_locked(this->omg->winmgr, this->surf, false);
    this->win->set_icon(this->win, this->surf);
    this->tex = this->ren->tex_create(this->ren, NULL, &OMG_FPOINT_MAKE(200, 200), OMG_TEXTURE_ACCESS_TARGET, true);
    this->sin_mul = 0.4;
    this->tex2 = OMG_REN_TEXTURE_FROM_FILE(this->ren, &OMG_STRING_MAKE_STATIC("assets/sprite.png"));
    this->ren->tex_set_scale_mode(this->ren, this->tex2, OMG_SCALE_MODE_NEAREST);
    this->clock->init(this->clock, true);
    this->clock->wait_for_limit = false;
    this->win->set_title(this->win, &OMG_STRING_MAKE_STATIC("Test Window"));
    this->audio->mus_set_volume(this->audio, this->mus, 0.1f);
    this->audio->snd_set_volume(this->audio, this->sound, 0.2f);
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
