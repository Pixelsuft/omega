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
    this->omg->log_info_str(this->omg, "1 123 456\n", 0);
    /*this->omg->log_info_str(this->omg, "1 123 456", 0);
    this->omg->log_info_str(this->omg, "1 123 456", 0);
    this->omg->log_info_str(this->omg, "1 123 456\n", 0);*/
    this->omg->destroy(this->omg);
}

int omega_main(OMG_EntryData* data) {
    App app;
    app_init(&app, data);
    if (app.exit_code)
        return app.exit_code;
    return app.exit_code;
}
