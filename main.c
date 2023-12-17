#include <omega/omega.h>
#include <omega/entry.h>
#include <omega/omega_win.h>
#include <omega/omega_sdl2.h>
#include <omega/memory_raylib.h>
#include <omega/omega_raylib.h>

typedef struct {
    OMG_Omega* omg;
    OMG_Window* win;
    int exit_code;
} App;

int omega_main(OMG_EntryData* data);
OMG_MAKE_MAIN(omega_main)

void app_init(App* this, OMG_EntryData* data) {
    this->exit_code = 1;
/*#if OMG_SUPPORT_RAYLIB
    this->omg = (OMG_Omega*)omg_raylib_create(data);
#elif OMG_SUPPORT_SDL2
    this->omg = (OMG_Omega*)omg_sdl2_create(data);
#else
    this->omg = (OMG_Omega*)omg_win_create(data);
#endif*/
    // this->omg = (OMG_Omega*)omg_sdl2_create(data);
    this->omg = (OMG_Omega*)omg_win_create(data);
    if (OMG_ISNULL(this->omg)) {
        return;
    }
    if (this->omg->omg_init(this->omg)) {
        return;
    }
    if (this->omg->app_init(this->omg)) {
        // TODO: auto cleanup function
        this->omg->destroy(this->omg);
        return;
    }
    this->win = this->omg->window_alloc(this->omg);
    if (OMG_ISNULL(this->win)) {
        this->omg->destroy(this->omg);
        return;
    }
    this->win->default_init(this->win);
    this->win->show(this->win);
    OMG_INFO(this->omg, 1337.228f, L" win32 is shit btw ", 228.1337, " 1", 228, "1 0x", (void*)this->omg);
    this->win->destroy(this->win);
    this->omg->window_free(this->omg, this->win);
    this->omg->app_quit(this->omg);
    this->omg->destroy(this->omg);
    this->exit_code = 0;
}

int omega_main(OMG_EntryData* data) {
    App app;
    app_init(&app, data);
    if (app.exit_code)
        return app.exit_code;
    return app.exit_code;
}
