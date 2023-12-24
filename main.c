#include <omega/omega.h>
#include <omega/entry.h>
#include <omega/omega_win.h>
#include <omega/omega_sdl2.h>
#include <omega/omega_raylib.h>
#include <omega/window_win.h>

typedef struct {
    OMG_Omega* omg;
    OMG_Window* win;
    int exit_code;
} App;

int omega_main(OMG_EntryData* data);
OMG_MAKE_MAIN(omega_main)

void app_on_destroy(OMG_EventLoopStop* event) {
    App* this = (App*)(((OMG_Event*)event)->data);
    // everything will be cleaned up automaticly
    // this->win->destroy(this->win);
    // this->omg->window_free(this->omg, this->win);
    this->omg->app_quit(this->omg);
    OMG_INFO(
        this->omg,
        "Exit. Number of extra allocations: ",
        (int)this->omg->mem->get_alloc_count(this->omg->mem) - 6
    );
    this->omg->destroy(this->omg);
    this->exit_code = 0;
}

void app_on_update(OMG_EventUpdate* event) {
#if OMG_SUPPORT_WIN
    App* this = (App*)(((OMG_Event*)event)->data);
    if (this->omg->type == OMG_OMEGA_TYPE_WIN) {
        OMG_OmegaWin* omg = ((OMG_OmegaWin*)this->omg);
        OMG_WindowWin* win = ((OMG_WindowWin*)this->win);
        omg->u32->InvalidateRect(win->hwnd, NULL, TRUE);
        omg->u32->UpdateWindow(win->hwnd);
        omg->k32->Sleep(5);
    }
#else
    OMG_UNUSED(event);
#endif
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
        this->omg->destroy(this->omg);
        return;
    }
    this->win->default_init(this->win);
    this->omg->event_arg = this;
    this->omg->on_update = app_on_update;
    this->omg->on_loop_stop = app_on_destroy;
    this->win->set_title(this->win, &OMG_STRING_MAKE_STATIC("Test Window"));
    OMG_INFO(this->omg, 1337.228f, L" win32 is shit btw ", 228.1337, " 1", 228, "1 0x", (void*)this->omg);
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
