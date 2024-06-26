#include <loader.h>
#include <aamain.h>
#include <omega/api_win.h>

void loader_clean(Loader* this) {
    // Clean loaded files
    App* app = this->_app;
    for (int i = 0; i < this->tex_count; i++) {
        if (OMG_ISNOTNULL(this->tex[i]))
            app->ren->tex_destroy(app->ren, this->tex[i]);
    }
    this->tex_count = 0;
    omg_ldtk_destroy(&this->mp[0]);
    omg_bmfont_destroy(&this->fnt[0]);
    app->au->mus_destroy(app->au, this->mus[0]);
    app->au->mus_destroy(app->au, this->mus[1]);
    app->au->snd_destroy(app->au, this->snd[0]);
    app->au->snd_destroy(app->au, this->snd[1]);
    this->mus_count = 0;
    this->snd_count = 0;
    this->fnt_count = 0;
    this->mp_count = 0;
}

void loader_clean_images(Loader* this) {
    // Clean surfaces since we converted all of them into textures
    App* app = this->_app;
    for (int i = 0; i < this->img_count; i++) {
        if (OMG_ISNOTNULL(this->surf[i]))
            app->omg->winmgr->surf_destroy(app->omg->winmgr, this->surf[i]);
    }
    this->img_count = 0;
}

void loader_img_load(Loader* this, const OMG_String* path) {
    // Load surface. We will then convert it into a texture
    App* app = this->_app;
    this->surf[this->img_count] = app_load_surf(app, path);
    this->img_count++;
    this->progress++;
    if (!OMG_IS_EMSCRIPTEN && 0)
        app->omg->delay(app->omg, 0.1);
}

void loader_fnt_load(Loader* this, const OMG_String* path) {
    // Load bmfont. Most of the code is file reading and error checks
    App* app = this->_app;
    OMG_String res_path;
    if (omg_string_init_dynamic(&res_path, &app->bp)) {
        this->progress++;
        this->fnt_count++;
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
    // Hack: we pass NULL page here
    omg_bmfont_init(&app->ld.fnt[this->fnt_count], NULL, app->ren, buf, (size_t)sz);
    OMG_FREE(app->omg->mem, buf);
    file->destroy(file);
    this->fnt_count++;
    this->progress++;
}

void loader_map_load(Loader* this, const OMG_String* path) {
    // Load map (way like bmfont)
    App* app = this->_app;
    OMG_String res_path;
    if (omg_string_init_dynamic(&res_path, &app->bp)) {
        this->progress++;
        this->mp_count++;
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

void loader_music_load(Loader* this, const OMG_String* path) {
    // Load music
    App* app = this->_app;
    OMG_String res_path;
    if (omg_string_init_dynamic(&res_path, &app->bp)) {
        this->mus_count++;
        this->progress++;
        return;
    }
    bool add_res = omg_string_add_char_p(&res_path, ASSETS_DIR) || omg_string_add_char(&res_path, OMG_PATH_DELIM) || omg_string_add(&res_path, path);
    this->mus[this->mus_count] = add_res ? NULL : app->au->mus_from_fp(app->au, NULL, &res_path, OMG_AUDIO_FORMAT_MP3);
    omg_string_destroy(&res_path);
    this->mus_count++;
    this->progress++;
}

void loader_sound_load(Loader* this, const OMG_String* path) {
    // Load sound
    App* app = this->_app;
    OMG_String res_path;
    if (omg_string_init_dynamic(&res_path, &app->bp)) {
        this->snd_count++;
        this->progress++;
        return;
    }
    bool add_res = omg_string_add_char_p(&res_path, ASSETS_DIR) || omg_string_add_char(&res_path, OMG_PATH_DELIM) || omg_string_add(&res_path, path);
    this->snd[this->snd_count] = add_res ? NULL : app->au->snd_from_fp(app->au, NULL, &res_path, OMG_AUDIO_FORMAT_OGG);
    omg_string_destroy(&res_path);
    this->snd_count++;
    this->progress++;
}

void loader_update(Loader* this) {
    App* app = this->_app;
    // Convert surfaces into textures. We can do this only in main thread,
    // since textures are stored in a GPU
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
    // Load some files
    this->total_count = 16;
    loader_img_load(this, &OMG_STR("goldFont-uhd.png"));
    loader_img_load(this, &OMG_STR("tiles1.png"));
    loader_img_load(this, &OMG_STR("bgBroccoli.png"));
    loader_img_load(this, &OMG_STR("kid.png"));
    loader_img_load(this, &OMG_STR("clouds.png"));
    loader_map_load(this, &OMG_STR("map1_map.txt"));
    loader_fnt_load(this, &OMG_STR("goldFont-uhd.fnt"));
    loader_music_load(this, &OMG_STR("menu.mp3"));
    loader_music_load(this, &OMG_STR("game.mp3"));
    loader_sound_load(this, &OMG_STR("jump1.ogg"));
    loader_sound_load(this, &OMG_STR("jump2.ogg"));
    this->loaded_images = true;
    // Clean surfaces
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
    // Let's try to create a thread. Run in main thread, if we can't
    if (!OMG_IS_EMSCRIPTEN && (app->omg->type != OMG_OMEGA_TYPE_WIN))
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
    // Set default values
    this->_app = _app;
    this->total_count = 999999;
    this->progress = 0;
    this->img_count = this->fnt_count = 0;
    this->tex_count = 0;
    this->mp_count = 0;
    this->mus_count = 0;
    this->snd_count = 0;
    this->loaded_images = false;
    this->thread_safe = true;
    this->finished = false;
    this->thr = NULL;
    app->omg->std->memset(this->tex, 0, sizeof(this->tex));
    app->omg->std->memset(this->surf, 0, sizeof(this->surf));
    app->omg->std->memset(this->fnt, 0, sizeof(this->fnt));
    app->omg->std->memset(this->mp, 0, sizeof(this->mp));
    app->omg->std->memset(this->mus, 0, sizeof(this->mus));
    app->omg->std->memset(this->snd, 0, sizeof(this->snd));
}
