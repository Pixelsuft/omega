#include <omega/omega.h>
#include <omega/entry.h>
#include <omega/winapi.h>
#include <omega/omega_win.h>
#include <omega/memory_win.h>
#include <omega/sdl2.h>

typedef struct {
    OMG_Omega* omg;
    int exit_code;
} App;

int omega_main(OMG_EntryData* data);
OMG_MAKE_MAIN(omega_main)

void app_init(App* this, OMG_EntryData* data) {
    this->exit_code = 0;
#if 1
    this->omg = (OMG_Omega*)omg_win_create(data);
#else
    this->omg = (OMG_Omega*)omg_win_create(data);
#endif
    if (OMG_ISNULL(this->omg)) {
        this->exit_code = 1;
        return;
    }
    if (this->omg->default_init(this->omg)) {
        this->exit_code = 1;
        return;
    }
    OMG_Sdl2* sdl2 = OMG_MALLOC(this->omg->mem, sizeof(OMG_Sdl2));
    omg_sdl2_dll_load(sdl2, NULL);
    OMG_INFO(this->omg, "SDL Version: ", (int)sdl2->ver.major, ".", (int)sdl2->ver.minor, ".", (int)sdl2->ver.patch);
    OMG_INFO(this->omg, 1337.228f, L" win32 is shit btw ", 228.1337, " 1", 228, "1 0x", (void*)this->omg);
    omg_sdl2_dll_free(sdl2);
    OMG_FREE(this->omg->mem, sdl2);
    this->omg->destroy(this->omg);
}

int omega_main(OMG_EntryData* data) {
    App app;
    app_init(&app, data);
    if (app.exit_code)
        return app.exit_code;
    return app.exit_code;
}
