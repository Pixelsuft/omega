#include <omega/omega.h>
#include <omega/entry.h>
#include <omega/omega_win.h>
#include <omega/omega_sdl2.h>
#include <omega/memory_raylib.h>
#include <omega/omega_raylib.h>

typedef struct {
    OMG_Omega* omg;
    int exit_code;
} App;

int omega_main(OMG_EntryData* data);
OMG_MAKE_MAIN(omega_main)

void app_init(App* this, OMG_EntryData* data) {
    this->exit_code = 0;
#if OMG_SUPPORT_SDL2
    this->omg = (OMG_Omega*)omg_sdl2_create(data);
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
    OMG_Raylib rl;
    omg_raylib_dll_load(&rl, NULL);
    OMG_Memory* mem = (OMG_Memory*)omg_memory_raylib_create(this->omg, &rl);
    OMG_INFO(this->omg, 1337.228f, (void*)mem, L" win32 is shit btw ", 228.1337, " 1", 228, "1 0x", (void*)this->omg);
    this->omg->destroy(this->omg);
}

int omega_main(OMG_EntryData* data) {
    App app;
    app_init(&app, data);
    if (app.exit_code)
        return app.exit_code;
    return app.exit_code;
}
