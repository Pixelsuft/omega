#include <omega/omega.h>
#include <omega/entry.h>
#include <omega/winapi.h>
#include <omega/omega_win.h>
#include <omega/memory_win.h>

typedef struct {
    OMG_Omega* omg;
    int exit_code;
} App;

int omega_main(OMG_EntryData* data);
OMG_MAKE_MAIN(omega_main)

void app_init(App* this, OMG_EntryData* data) {
    this->exit_code = 0;
    this->omg = (OMG_Omega*)omg_win_create(data);
    if (OMG_ISNULL(this->omg)) {
        this->exit_code = 1;
        return;
    }
    if (omg_win_init((OMG_OmegaWin*)this->omg)) {
        this->exit_code = 1;
        return;
    }
    OMG_INFO(
        this->omg, "Windows ", ((OMG_OmegaWin*)this->omg)->win_major_ver, " build ",
        ((OMG_OmegaWin*)this->omg)->win_build_number
    );
    OMG_INFO(this->omg, 1337.228f, " ", 228.1337, " 1", 228, "1 0x", (void*)this->omg);
    this->omg->destroy(this->omg);
}

int omega_main(OMG_EntryData* data) {
    App app;
    app_init(&app, data);
    if (app.exit_code)
        return app.exit_code;
    return app.exit_code;
}
