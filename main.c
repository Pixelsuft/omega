#include <omega/omega.h>
#include <omega/entry.h>
#include <omega/omega_win.h>
#include <omega/omega_sdl2.h>
#include <omega/omega_raylib.h>
#include <omega/window_win.h>

typedef struct {
    OMG_Omega* omg;
    OMG_Window* win;
    OMG_Renderer* ren;
    int exit_code;
} App;

int omega_main(OMG_EntryData* data);
OMG_MAKE_MAIN(omega_main)

void app_on_destroy(OMG_EventLoopStop* event) {
    App* this = (App*)(((OMG_Event*)event)->data);
    // everything will be cleaned up automaticly
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

void app_on_update(OMG_EventUpdate* event) {
    App* this = (App*)(((OMG_Event*)event)->data);
    OMG_UNUSED(this);
}

void app_on_paint(OMG_EventPaint* event) {
    App* this = (App*)(((OMG_Event*)event)->data);
    this->ren->begin(this->ren);
    this->ren->clear(this->ren, &OMG_COLOR_MAKE_RGB(100, 100, 100));
    this->ren->flip(this->ren);
}

void app_init(App* this, OMG_EntryData* data) {
    this->exit_code = 1;
#if OMG_SUPPORT_RAYLIB
    this->omg = (OMG_Omega*)omg_raylib_create(data);
#endif
#if OMG_SUPPORT_SDL2
    this->omg = (OMG_Omega*)omg_sdl2_create(data);
#endif
#if OMG_SUPPORT_WIN
    this->omg = (OMG_Omega*)omg_win_create(data);
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
        this->omg->destroy(this->omg);
        return;
    }
    this->win->resizable = false;
    if (this->win->default_init(this->win)) {
        this->omg->destroy(this->omg);
        return;
    }
    if (this->win->renderer_alloc(this->win) || this->win->ren->init(this->win->ren)) {
        this->omg->destroy(this->omg);
        return;
    }
    this->ren = this->win->ren;
    this->omg->event_arg = this;
    this->omg->on_update = app_on_update;
    this->omg->on_paint = app_on_paint;
    this->omg->on_loop_stop = app_on_destroy;
    this->win->set_title(this->win, &OMG_STRING_MAKE_STATIC("Test Window"));
    OMG_INFO(this->omg, 1337.228f, L" win32 is shit btw ", 228.1337, " 1", 228, "1 0x", (void*)this->omg);
    this->win->show(this->win, true);
    this->win->set_sys_button(this->win, OMG_WIN_SYS_BUTTON_CLOSE, false);
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
