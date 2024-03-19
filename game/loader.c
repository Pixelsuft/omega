#include <loader.h>
#include <aamain.h>
#include <omega/api_win.h>

void loader_clean(Loader* this) {
    App* app = this->_app;
    for (int i = 0; i < this->img_count; i++) {
        if (OMG_ISNOTNULL(this->tex[i]))
            app->ren->tex_destroy(app->ren, this->tex[i]);
    }
}

void loader_img_load(Loader* this, const OMG_String* path) {
    App* app = this->_app;
    this->surf[this->img_count] = app_load_surf(app, path);
    this->img_count++;
    this->progress++;
}

int loader_thread(void* data) {
    Loader* this = data;
    loader_img_load(this, &OMG_STR("goldFont-uhd.png"));
    loader_img_load(this, &OMG_STR("tiles1.png"));
    loader_img_load(this, &OMG_STR("bgBroccoli.png"));
    return 0;
}

void loader_run(Loader* this) {
    App* app = _app;
    OMG_THREAD_CREATE(this->thr, app->omg, loader_thread, &OMG_STR("ldrthr"), this, 0);
    if (OMG_ISNULL(this->thr)) {
        loader_thread(this);
    }
    else {
        // TODO: remove this
        app->omg->thread_detach(app->omg, this->thr);
    }
}

void loader_init(Loader* this, void* _app) {
    App* app = _app;
    this->_app = _app;
    this->total_count = 3;
    this->progress = 0;
    this->img_count = 0;
    this->thr = NULL;
    app->omg->std->memset(this->tex, 0, sizeof(this->tex));
    app->omg->std->memset(this->surf, 0, sizeof(this->surf));
}
