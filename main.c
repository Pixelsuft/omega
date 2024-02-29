#include <omega/omega.h>
#include <omega/entry.h>
#include <omega/api_win.h>
#include <omega/scene.h>
#include <omega/api_sdl2.h>
#if OMG_DEBUG && OMG_HAS_STD
#include <stdio.h>
#endif
#define scene_base ((OMG_Scene*)this->sc)
// SDL_ttf doesn't compile on UWP (((
#define SUPPORT_FONT !OMG_IS_UWP
#define MAX_OBJECTS 20

typedef struct {
    OMG_Scene parent;
    OMG_Object* objects[MAX_OBJECTS];
} TestScene;

typedef struct {
    OMG_Omega* omg;
    OMG_Window* win;
    OMG_Renderer* ren;
    OMG_Audio* audio;
    OMG_Clock* clock;
    OMG_FontMgr* fnt;
    OMG_Font* fps_font;
    OMG_SceneMgr* sm;
    TestScene* sc;
    OMG_String fps_str;
    char fps_buf[20];
    int exit_code;
} App;

int omega_main(OMG_EntryData* data);
OMG_MAIN_MAKE(omega_main)

void app_on_destroy(OMG_EventLoopStop* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    omg_scenemgr_scene_destroy(this->sm, this->sc);
    omg_scenemgr_destroy(this->sm);
    OMG_FREE(this->omg->mem, this->sc);
    OMG_FREE(this->omg->mem, this->sm);
#if SUPPORT_FONT
    this->fnt->font_destroy(this->fnt, this->fps_font);
#endif
    this->omg->app_quit(this->omg);
    OMG_INFO(
        this->omg,
        "Exit. Number of allocations: ",
        (int)this->omg->mem->get_alloc_count(this->omg->mem)
    );
    this->omg->destroy(this->omg);
    this->exit_code = 0;
}

bool scene_on_destroy(TestScene* scene) {
    App* this = (App*)((OMG_Scene*)scene)->data;
    OMG_INFO(this->omg, "Scene destroy");
    return false;
}

bool scene_on_update(TestScene* scene) {
    App* this = (App*)((OMG_Scene*)scene)->data;
    if (this->clock->update(this->clock)) {
        this->omg->enable_paint = false;
        scene_base->enable_paint = false;
        return false;
    }
    if (this->clock->dt > 1.0)
        this->clock->dt = 1.0;
    this->omg->enable_paint = true;
    scene_base->enable_paint = true;
    this->fps_str.len = 5;
    omg_string_add_int(&this->fps_str, this->clock->get_fps(this->clock));
    scene_base->dt = this->clock->dt;
    for (size_t i = 0; i < MAX_OBJECTS; i++) {
        OMG_Object* obj = scene->objects[i];
        if (OMG_ISNULL(obj))
            continue;
    }
    // OMG_INFO(this->omg, "Scene update");
    return false;
}

bool scene_on_paint(TestScene* scene) {
    App* this = (App*)((OMG_Scene*)scene)->data;
    this->ren->begin(this->ren);
    this->ren->clear(this->ren, &OMG_COLOR_MAKE_RGB(100, 50, 50));
#if SUPPORT_FONT
    this->ren->font_render_to(this->ren, NULL, this->fps_font, &this->fps_str, NULL, &OMG_COLOR_MAKE_RGB(0, 255, 255), NULL);
#endif
    this->ren->flip(this->ren);
    // OMG_INFO(this->omg, "Scene paint");
    return false;
}

bool scene_on_run(TestScene* scene) {
    App* this = (App*)((OMG_Scene*)scene)->data;
    this->omg->clock->reset(this->omg->clock);
    OMG_INFO(this->omg, "Scene run");
    return false;
}

bool scene_on_init(TestScene* scene) {
    App* this = (App*)((OMG_Scene*)scene)->data;
    OMG_BEGIN_POINTER_CAST();
    scene_base->on_destroy = scene_on_destroy;
    scene_base->on_update = scene_on_update;
    scene_base->on_paint = scene_on_paint;
    scene_base->on_run = scene_on_run;
    OMG_END_POINTER_CAST();
    this->omg->std->memset(scene->objects, 0, sizeof(scene->objects));
    OMG_INFO(this->omg, "Scene init");
    return false;
}

void app_on_update(OMG_EventUpdate* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    this->audio->update(this->audio);
}

void app_on_paint(OMG_EventPaint* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    OMG_UNUSED(this);
}

void app_on_expose(OMG_EventExpose* event) {
    // app_on_update((OMG_EventUpdate*)event);
    app_on_paint((OMG_EventPaint*)event);
}

void app_on_key_down(OMG_EventKeyboard* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    if (event->code == OMG_SCANCODE_Q) {
        this->omg->auto_loop_stop(this->omg);
        return;
    }
    else if (event->code == OMG_SCANCODE_V)
        this->ren->set_vsync(this->ren, !this->win->vsync);
    else if (event->code == OMG_SCANCODE_E)
        this->ren->aa = !this->ren->aa;
    else if (event->code == OMG_SCANCODE_T)
        this->fps_font->aa = !this->fps_font->aa;
    else if (event->code == OMG_SCANCODE_L)
        this->win->set_window_mode(
            this->win,
            (this->win->window_mode != OMG_WIN_MODE_WINDOW) ? OMG_WIN_MODE_WINDOW : OMG_WIN_MODE_DESKTOP_FULLSCREEN
        );
    else if (event->code == OMG_SCANCODE_P)
        this->win->mouse_set_system_cursor(this->win, OMG_SYSTEM_CURSOR_CROSSHAIR);
    else if (event->code == OMG_SCANCODE_LEFTBRACKET)
        this->win->mouse_set_system_cursor(this->win, OMG_SYSTEM_CURSOR_ARROW);
    else if (event->is_pressed) {
        if (event->code == OMG_SCANCODE_U)
            this->win->mouse_set_rel(this->win, 2); // 0 - Off, 1 - On, 2 - Toggle, Other - Query
        else if (event->code == OMG_SCANCODE_I)
            this->win->set_grab(this->win, 2);
        else if (event->code == OMG_SCANCODE_O)
            this->win->cursor_set_shown(this->win, 2);
        else if (event->code == OMG_SCANCODE_B)
            this->win->set_opacity(this->win, this->win->get_opacity(this->win) > 0.5f ? 0.5f : 1.0f);
    }
}

void app_init(App* this, OMG_EntryData* data) {
    this->exit_code = 1;
#if 0
#elif OMG_SUPPORT_SDL2
    this->omg = omg_create_by_type(data, OMG_OMEGA_TYPE_SDL2);
#elif OMG_SUPPORT_RAYLIB
    this->omg = omg_create_by_type(data, OMG_OMEGA_TYPE_RAYLIB);
#elif OMG_SUPPORT_WIN
    this->omg = omg_create_by_type(data, OMG_OMEGA_TYPE_WIN);
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
    this->win->size.w = 1024.0f;
    this->win->size.h = 768.0f;
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
    OMG_String temp_env = this->omg->env_get(this->omg, &OMG_STRING_MAKE_STATIC("OMG_SOFTWARE_RENDERER"));
    int force_ren_driver = OMG_REN_DRIVER_AUTO;
    if (temp_env.type >= 0)
        force_ren_driver = OMG_REN_DRIVER_SOFTWARE;
    omg_string_destroy(&temp_env);
    if (
        this->win->renderer_alloc(this->win) ||
        !(this->win->ren->driver = force_ren_driver) ||
        this->win->ren->init(this->win->ren)
    ) {
        OMG_ERROR(this->omg, "OMG Renderer Init Fail");
        this->omg->destroy(this->omg);
        return;
    }
    if ((!OMG_IS_EMSCRIPTEN && !OMG_IS_ANDROID && 0) || this->omg->type == OMG_OMEGA_TYPE_WIN)
        this->omg->audio_type = OMG_AUDIO_TYPE_FMOD;
    if (sizeof(void*) == 4 && this->omg->audio_type == OMG_AUDIO_TYPE_FMOD)
        this->omg->audio_type = OMG_AUDIO_TYPE_SDL2;
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
    this->ren->soft_offset = true;
    this->ren->aa = !OMG_IS_EMSCRIPTEN;
#if SUPPORT_FONT
    this->fps_font = this->fnt->font_from_fp(this->fnt, NULL, &OMG_STRING_MAKE_STATIC("assets/segoeuib.ttf"), -1, 32.0f);
    this->fps_font->wrapping = false;
    this->fps_font->text_type = OMG_FONT_TEXT_TYPE_TEXT;
#endif
    this->omg->std->memcpy(this->fps_buf, "FPS:               \0", 20);
    this->fps_str = OMG_STRING_MAKE_BUFFER_A(this->fps_buf);
    this->clock = this->omg->clock;
    this->win->allow_alt = false;
    this->omg->event_arg = this;
    this->omg->on_update = app_on_update;
    this->omg->on_paint = app_on_paint;
    this->omg->on_expose = app_on_expose;
    this->omg->on_loop_stop = app_on_destroy;
    this->omg->on_key_down = app_on_key_down;
    this->win->set_min_size(this->win, &OMG_FPOINT_MAKE(320, 200));
    temp_env = this->omg->env_get(this->omg, &OMG_STRING_MAKE_STATIC("OMG_MS_CLOCK"));
    this->clock->init(this->clock, temp_env.type >= 0);
    omg_string_destroy(&temp_env);
    this->clock->wait_for_limit = false;
    this->win->set_title(this->win, &OMG_STRING_MAKE_STATIC("Test Window"));
    this->sm = OMG_MALLOC(this->omg->mem, sizeof(OMG_SceneMgr));
    this->sc = OMG_MALLOC(this->omg->mem, sizeof(TestScene));
    omg_scenemgr_init(this->sm, this->ren);
    omg_scenemgr_scene_fill(this->sm, this->sc);
    OMG_BEGIN_POINTER_CAST();
    scene_base->on_init = scene_on_init;
    OMG_END_POINTER_CAST();
    omg_scenemgr_scene_init(this->sm, this->sc, this);
    OMG_INFO(this->omg, "Hello world ", 1337.228f, " ", 228.1337, " 1", 228, "1 0x", (void*)this->omg);
    this->win->show(this->win, true);
    this->win->mouse_set_system_cursor(this->win, OMG_SYSTEM_CURSOR_DEFAULT);
    omg_scenemgr_scene_run(this->sm, this->sc);
    // this->clock->reset(this->clock);
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
