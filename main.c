#include <omega/omega.h>
#include <omega/entry.h>
#include <omega/winapi.h>
#include <omega/omega_win.h>
#include <omega/memory_win.h>
#include <omega/sdl2.h>
#include <omega/memory_sdl2.h>

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
    OMG_Memory* mem = (OMG_Memory*)omg_memory_sdl2_create(this->omg, sdl2);
    int* buf = OMG_MALLOC(mem, sizeof(int) * 2);
    OMG_REALLOC(mem, buf, 3 * sizeof(int));
    buf[0] = 1;
    buf[1] = 2;
    buf[2] = 3;
    OMG_INFO(this->omg, buf[0], ", ", buf[1], ", ", buf[2]);
    OMG_FREE(mem, buf);
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
