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
    omg_ldtk_destroy(&this->mp[0]);
    omg_bmfont_destroy(&this->fnt[0]);
    this->fnt_count = 0;
    this->mp_count = 0;
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
    // app->omg->delay(app->omg, 0.2);
}

void loader_fnt_load(Loader* this, const OMG_String* path) {
    App* app = this->_app;
    OMG_String res_path;
    if (omg_string_init_dynamic(&res_path, &app->bp)) {
        return;
    }
    bool add_res = omg_string_add_char_p(&res_path, ASSETS_DIR) || omg_string_add_char(&res_path, OMG_PATH_DELIM) || omg_string_add(&res_path, path);
    OMG_File* file = add_res ? NULL : app->omg->file_from_fp(app->omg, NULL, &res_path, OMG_FILE_MODE_RB);
    omg_string_destroy(&res_path);
    if (OMG_ISNULL(file)) {
        this->progress++;
        this->fnt_count++;
        return;
    }
    int64_t sz = file->get_size(file);
    if (sz <= 0) {
        file->destroy(file);
        this->progress++;
        this->fnt_count++;
        return;
    }
    char* buf = OMG_MALLOC(app->omg->mem, sz + 10);
    if (OMG_ISNULL(buf)) {
        file->destroy(file);
        this->progress++;
        this->fnt_count++;
        return;
    }
    file->read(file, buf, 1, (size_t)(sz + 2));
    omg_bmfont_init(&app->ld.fnt[this->fnt_count], NULL, app->ren, buf, (size_t)sz);
    OMG_FREE(app->omg->mem, buf);
    file->destroy(file);
    this->fnt_count++;
    this->progress++;
}

void loader_map_load(Loader* this, const OMG_String* path) {
    App* app = this->_app;
    OMG_String res_path;
    if (omg_string_init_dynamic(&res_path, &app->bp)) {
        return;
    }
    bool add_res = omg_string_add_char_p(&res_path, ASSETS_DIR) || omg_string_add_char(&res_path, OMG_PATH_DELIM) || omg_string_add(&res_path, path);
    OMG_File* file = add_res ? NULL : app->omg->file_from_fp(app->omg, NULL, &res_path, OMG_FILE_MODE_RB);
    omg_string_destroy(&res_path);
    if (OMG_ISNULL(file)) {
        this->progress++;
        this->mp_count++;
        return;
    }
    int64_t sz = file->get_size(file);
    if (sz <= 0) {
        file->destroy(file);
        this->progress++;
        this->mp_count++;
        return;
    }
    char* buf = OMG_MALLOC(app->omg->mem, sz + 10);
    if (OMG_ISNULL(buf)) {
        file->destroy(file);
        this->progress++;
        this->mp_count++;
        return;
    }
    file->read(file, buf, 1, (size_t)(sz + 2));
    omg_ldtk_init(&app->ld.mp[this->mp_count], app->omg, buf, (size_t)sz);
    OMG_FREE(app->omg->mem, buf);
    file->destroy(file);
    this->mp_count++;
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
    this->total_count = 8;
    loader_img_load(this, &OMG_STR("goldFont-uhd.png"));
    loader_img_load(this, &OMG_STR("tiles1.png"));
    loader_img_load(this, &OMG_STR("bgBroccoli.png"));
    loader_map_load(this, &OMG_STR("map1_map.txt"));
    loader_fnt_load(this, &OMG_STR("goldFont-uhd.fnt"));
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
    this->fnt[0].page = this->tex[0]; // Hack
    this->thread_safe = true;
    this->finished = true;
    return 0;
}

void loader_run(Loader* this) {
    App* app = _app;
    this->thread_safe = false;
    if (app->omg->type != OMG_OMEGA_TYPE_WIN)
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
    this->total_count = 999999;
    this->progress = 0;
    this->img_count = this->fnt_count = 0;
    this->tex_count = 0;
    this->mp_count = 0;
    this->loaded_images = false;
    this->thread_safe = true;
    this->finished = false;
    this->thr = NULL;
    app->omg->std->memset(this->tex, 0, sizeof(this->tex));
    app->omg->std->memset(this->surf, 0, sizeof(this->surf));
    app->omg->std->memset(this->fnt, 0, sizeof(this->fnt));
    app->omg->std->memset(this->mp, 0, sizeof(this->mp));
}
