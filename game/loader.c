#include <loader.h>
#include <aamain.h>
#include <omega/api_win.h>

void loader_clean(Loader* this) {
    App* app = this->_app;
    for (int i = 0; i < this->tex_count; i++) {
        if (OMG_ISNOTNULL(this->tex[i]))
            app->ren->tex_destroy(app->ren, this->tex[i]);
    }
    this->tex_count = 0;
}

void loader_clean_images(Loader* this) {
    App* app = this->_app;
    for (int i = 0; i < this->img_count; i++) {
        if (OMG_ISNOTNULL(this->surf[i]))
            app->omg->winmgr->surf_destroy(app->omg->winmgr, this->surf[i]);
    }
    this->img_count = 0;
}

void loader_img_load(Loader* this, const OMG_String* path) {
    App* app = this->_app;
    this->surf[this->img_count] = app_load_surf(app, path);
    this->img_count++;
    this->progress++;
}

void loader_update(Loader* this) {
    App* app = this->_app;
    if (this->img_count > 0) {
        if (this->img_count > this->tex_count) {
            this->tex[this->tex_count] = app->ren->tex_from_surf(
                app->ren, NULL, this->surf[this->tex_count], false
            );
            this->tex_count++;
            this->progress++;
        }
    }
}

int loader_thread(void* data) {
    Loader* this = data;
    App* app = this->_app;
    loader_img_load(this, &OMG_STR("goldFont-uhd.png"));
    loader_img_load(this, &OMG_STR("tiles1.png"));
    loader_img_load(this, &OMG_STR("bgBroccoli.png"));
    this->loaded_images = true;
    if (!this->thread_safe) {
        while (this->tex_count < this->img_count) {
            app->omg->delay(app->omg, 0.01);
        }
        loader_clean_images(this);
    }
    else {
        for (size_t i = 0; i < 1000; i++) {
            loader_update(this);
        }
        loader_clean_images(this);
    }
    this->thread_safe = true;
    this->finished = true;
    return 0;
}

void loader_run(Loader* this) {
    App* app = _app;
    this->thread_safe = false;
    OMG_THREAD_CREATE(this->thr, app->omg, loader_thread, &OMG_STR("ldrthr"), this, 0);
    if (OMG_ISNULL(this->thr)) {
        this->thread_safe = true;
        loader_thread(this);
    }
    else {
        // Should I remove this?
        app->omg->thread_detach(app->omg, this->thr);
    }
}

void loader_init(Loader* this, void* _app) {
    App* app = _app;
    this->_app = _app;
    this->total_count = 6;
    this->progress = 0;
    this->img_count = 0;
    this->tex_count = 0;
    this->loaded_images = false;
    this->thread_safe = true;
    this->finished = false;
    this->thr = NULL;
    app->omg->std->memset(this->tex, 0, sizeof(this->tex));
    app->omg->std->memset(this->surf, 0, sizeof(this->surf));
}
