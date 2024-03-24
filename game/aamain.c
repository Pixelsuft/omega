#include <omega/omega.h>
#include <omega/entry.h>
#include <omega/api_win.h>
#include <omega/api_static.h>
#include <aamain.h>
#include <logo.h>

int omega_main(OMG_EntryData* data);
OMG_MAIN_MAKE(omega_main)

App* _app;

void app_on_destroy(OMG_EventLoopStop* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    omg_scenemgr_destroy(&this->sm);
    omg_string_destroy(&this->bp);
    loader_clean(&this->ld);
    this->omg->app_quit(this->omg);
    OMG_INFO(
        this->omg,
        "Exit. Number of allocations: ",
        (int)this->omg->mem->get_alloc_count(this->omg->mem)
    );
    this->omg->destroy(this->omg);
}

OMG_Texture* app_load_texture(App* this, const OMG_String* path) {
    OMG_String res_path;
    if (omg_string_init_dynamic(&res_path, &this->bp))
        return omg_renderer_dummy_tex_create(this->ren);
    bool add_res = omg_string_add_char_p(&res_path, ASSETS_DIR) || omg_string_add_char(&res_path, OMG_PATH_DELIM) || omg_string_add(&res_path, path);
    if (add_res) {
        omg_string_destroy(&res_path);
        return omg_renderer_dummy_tex_create(this->ren);
    }
    OMG_Texture* res = OMG_REN_TEXTURE_FROM_FILE(this->ren, &res_path);
    omg_string_destroy(&res_path);
    return res;
}

OMG_Surface* app_load_surf(App* this, const OMG_String* path) {
    OMG_String res_path;
    if (omg_string_init_dynamic(&res_path, &this->bp))
        return omg_winmgr_dummy_surf_create(this->omg->winmgr);
    bool add_res = omg_string_add_char_p(&res_path, ASSETS_DIR) || omg_string_add_char(&res_path, OMG_PATH_DELIM) || omg_string_add(&res_path, path);
    if (add_res) {
        omg_string_destroy(&res_path);
        return omg_winmgr_dummy_surf_create(this->omg->winmgr);
    }
    OMG_Surface* res = this->omg->winmgr->surf_from_fp(this->omg->winmgr, NULL, &res_path, OMG_IMG_FORMAT_PNG);
    omg_string_destroy(&res_path);
    return res;
}

void app_on_keyboard(OMG_EventKeyboard* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    OMG_UNUSED(this);
}

void app_on_resize(OMG_EventResize* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    this->sc.w = event->size.w / 800.0f;
    this->sc.h = event->size.h / 600.0f;
}

void app_draw_fps(App* this) {
    if (!this->show_fps)
        return;
    this->fps_str.len = 5;
    omg_string_add_int(&this->fps_str, this->clock->get_fps(this->clock));
    omg_bmfont_render(&this->ld.fnt[0], &this->fps_str, NULL);
}

bool app_init(App* this, OMG_EntryData* data) {
    int omg_backend = 0;
    omg_backend = OMG_OMEGA_TYPE_SDL2;
    this->omg = omg_create_by_type(data, omg_backend);
    if (OMG_ISNULL(this->omg) || this->omg->omg_init(this->omg))
        return true;
    if (
        this->omg->app_init(this->omg) ||
        this->omg->winmgr_alloc(this->omg) ||
        this->omg->winmgr->init(this->omg->winmgr) ||
        (OMG_ISNULL(this->win = this->omg->winmgr->window_alloc(this->omg->winmgr)))
    ) {
        OMG_ERROR(this->omg, "OMG Init Fail");
        this->omg->destroy(this->omg);
        return true;
    }
    this->win->resizable = true;
    if (this->win->default_init(this->win)) {
        OMG_ERROR(this->omg, "OMG Window Init Fail");
        this->omg->destroy(this->omg);
        return true;
    }
    if (
        this->omg->winmgr->image_loader_alloc(this->omg->winmgr) ||
        this->omg->winmgr->img->init(this->omg->winmgr->img)
    ) {
        OMG_ERROR(this->omg, "OMG Image Loader Init Fail");
        this->omg->destroy(this->omg);
        return true;
    }
    OMG_String temp_env = this->omg->env_get(this->omg, &OMG_STR("OMG_SOFTWARE_RENDERER"));
    int force_ren_driver = OMG_REN_DRIVER_AUTO;
    if (OMG_IS_WIN && 0)
        force_ren_driver = (this->omg->type == OMG_OMEGA_TYPE_WIN) ? OMG_REN_DRIVER_D3D11 : OMG_REN_DRIVER_OPENGL;
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
        return true;
    }
    this->omg->audio_type = OMG_AUDIO_TYPE_AUTO;
    if (this->omg->audio_alloc(this->omg) || this->omg->audio->init(this->omg->audio)) {
        OMG_ERROR(this->omg, "OMG Audio Init Fail");
        this->omg->destroy(this->omg);
        return false;
    }
    this->au = this->omg->audio;
    this->ren = this->win->ren;
    this->ren->soft_offset = true;
    this->ren->soft_scale = true;
    this->ren->aa = !OMG_IS_EMSCRIPTEN;
    this->ren->tex_default_scale_mode = OMG_SCALE_MODE_NEAREST;
    this->clock = this->omg->clock;
    this->win->allow_alt = false;
    this->omg->event_arg = this;
    temp_env = this->omg->env_get(this->omg, &OMG_STR("OMG_MS_CLOCK"));
    this->clock->init(this->clock, !(temp_env.type >= 0));
    omg_string_destroy(&temp_env);
    this->clock->wait_for_limit = false;
    this->win->set_title(this->win, &OMG_STR("Example game"));
    this->omg->on_loop_stop = app_on_destroy;
    this->omg->on_key_down = app_on_keyboard;
    this->omg->on_size_change = app_on_resize;
    this->bp = this->omg->get_cwd(this->omg, true);
    omg_scenemgr_init(&this->sm, this->ren);
    loader_init(&this->ld, this);
    this->fps_buf[0] = 'F';
    this->fps_buf[1] = 'P';
    this->fps_buf[2] = 'S';
    this->fps_buf[3] = ':';
    this->fps_buf[4] = ' ';
    this->fps_buf[5] = '\0';
    this->fps_str = OMG_STRING_MAKE_BUFFER(this->fps_buf);
    this->fps_str.size = 20;
    this->show_fps = OMG_DEBUG;
    return false;
}

void app_run(App* this) {
    LogoScene* lc = OMG_MALLOC(this->omg->mem, sizeof(LogoScene));
    omg_scenemgr_scene_fill(&this->sm, lc);
    OMG_BEGIN_POINTER_CAST();
    lc->parent.on_init = logo_scene_init;
    OMG_END_POINTER_CAST();
    omg_scenemgr_scene_init(&this->sm, lc, this);
    this->win->show(this->win, true);
    omg_scenemgr_scene_run(&this->sm, lc);
    this->omg->auto_loop_run(this->omg);
}

int omega_main(OMG_EntryData* data) {
    _app = omg_static_malloc(sizeof(App));
    if (OMG_ISNULL(_app))
        return 1;
    if (app_init(_app, data)) {
        omg_static_free(_app);
        return 1;
    }
    app_run(_app);
    omg_static_free(_app);
    return 0;
}
