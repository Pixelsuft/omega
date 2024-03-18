#include <aamain.h>
#include <omega/omega.h>
#include <omega/entry.h>
#include <omega/api_win.h>
#include <omega/api_static.h>

int omega_main(OMG_EntryData* data);
OMG_MAIN_MAKE(omega_main)

App* app;

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
    if (OMG_IS_WIN)
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
    this->au = this->omg->audio;
    this->ren = this->win->ren;
    this->ren->soft_offset = false;
    this->ren->aa = !OMG_IS_EMSCRIPTEN;
    this->clock = this->omg->clock;
    this->win->allow_alt = false;
    this->omg->event_arg = this;
    temp_env = this->omg->env_get(this->omg, &OMG_STR("OMG_MS_CLOCK"));
    this->clock->init(this->clock, !(temp_env.type >= 0));
    omg_string_destroy(&temp_env);
    this->clock->wait_for_limit = false;
    this->win->set_title(this->win, &OMG_STR("Example game"));
    // this->win->show(this->win, true);
    return false;
}

int omega_main(OMG_EntryData* data) {
    app = omg_static_malloc(sizeof(App));
    if (OMG_ISNULL(app))
        return 1;
    if (app_init(app, data)) {
        omg_static_free(app);
        return 1;
    }
    omg_static_free(app);
    return 0;
}
