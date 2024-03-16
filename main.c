#include <omega/omega.h>
#include <omega/entry.h>
#include <omega/api_win.h>
#include <omega/scene.h>
#include <omega/scene_objects.h>
#include <omega/array.h>
#include <omega/bmfont.h>
#include <omega/ldtk.h>
#if OMG_DEBUG && OMG_HAS_STD
#include <stdio.h>
#endif
#define scene_base ((OMG_Scene*)this->sc)
// SDL_ttf doesn't compile on UWP (((
#define SUPPORT_FONT !(OMG_IS_UWP || OMG_IS_ANDROID)
#define MAX_OBJECTS 20

typedef struct {
    OMG_Scene parent;
    OMG_Bmfont bmfont;
    OMG_Ldtk map;
    OMG_Color circle_color;
    OMG_FRect jumper_src;
    OMG_Texture* map_tex;
    OMG_Object* objects[MAX_OBJECTS];
    OMG_ObjectAnimSprite anim;
    OMG_AnimSpriteData anim_data;
    OMG_ObjectTimer timer;
    OMG_ObjectAnimTimer sin_timer;
    OMG_ObjectAnimTimer x_timer;
} TestScene;

typedef struct {
    OMG_Omega* omg;
    OMG_Window* win;
    OMG_Renderer* ren;
    OMG_Audio* audio;
    OMG_Clock* clock;
    OMG_FontMgr* fnt;
    OMG_Font* fps_font;
    OMG_SceneMgr* sm;
    TestScene* sc;
    OMG_Texture* font_tex;
    OMG_Texture* tilemap1;
    OMG_Texture* tilemap2;
    OMG_Texture* jumper_anims;
    OMG_String fps_str;
    char fps_buf[20];
    int exit_code;
} App;

int omega_main(OMG_EntryData* data);
OMG_MAIN_MAKE(omega_main)

void app_on_destroy(OMG_EventLoopStop* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    omg_scenemgr_scene_destroy(this->sm, this->sc);
    omg_scenemgr_destroy(this->sm);
    OMG_FREE(this->omg->mem, this->sc);
    OMG_FREE(this->omg->mem, this->sm);
    this->ren->tex_destroy(this->ren, this->jumper_anims);
    this->ren->tex_destroy(this->ren, this->tilemap2);
    this->ren->tex_destroy(this->ren, this->tilemap1);
    this->ren->tex_destroy(this->ren, this->font_tex);
#if SUPPORT_FONT
    this->fnt->font_destroy(this->fnt, this->fps_font);
#endif
    this->omg->app_quit(this->omg);
    OMG_INFO(
        this->omg,
        "Exit. Number of allocations: ",
        (int)this->omg->mem->get_alloc_count(this->omg->mem)
    );
    this->omg->destroy(this->omg);
    this->exit_code = 0;
}

bool scene_on_destroy(TestScene* scene) {
    App* this = (App*)((OMG_Scene*)scene)->data;
    omg_anim_sprite_state_destroy(&scene->anim_data.states.data[0], this->omg);
    omg_anim_sprite_data_destroy(&scene->anim_data);
    this->ren->tex_destroy(this->ren, scene->map_tex);
    omg_ldtk_destroy(&scene->map);
    omg_bmfont_destroy(&scene->bmfont);
    OMG_INFO(this->omg, "Scene destroy");
    return false;
}

bool scene_on_update(TestScene* scene) {
    App* this = (App*)((OMG_Scene*)scene)->data;
    if (this->clock->update(this->clock)) {
        this->omg->enable_paint = false;
        scene_base->enable_paint = false;
        return false;
    }
    if (this->clock->dt > 1.0)
        this->clock->dt = 1.0;
    this->omg->enable_paint = true;
    scene_base->enable_paint = true;
    this->fps_str.len = 5;
    omg_string_add_int(&this->fps_str, this->clock->get_fps(this->clock));
    scene_base->dt = this->clock->dt;
    for (size_t i = 0; i < MAX_OBJECTS; i++) {
        OMG_Object* obj = scene->objects[i];
        if (OMG_ISNULL(obj))
            continue;
        if (OMG_ISNOTNULL(obj->on_update))
            obj->on_update(obj, this->sm->cur_scene);
    }
    if (scene->timer.triggered > 0) {
        scene->timer.triggered--;
        if (scene->circle_color.b >= 255.0f) {
            scene->circle_color.b = 0.0f;
            scene->circle_color.g = 255.0f;
        }
        else {
            scene->circle_color.b = 255.0f;
            scene->circle_color.g = 0.0f;
        }
    }
    // OMG_INFO(this->omg, "Scene update");
    return false;
}

bool scene_on_paint(TestScene* scene) {
    App* this = (App*)((OMG_Scene*)scene)->data;
    this->ren->begin(this->ren);
    this->ren->clear(this->ren, &OMG_COLOR_MAKE_RGB(25, 25, 25));
    // Render tilemap
    this->ren->set_scale(this->ren, NULL, &OMG_FPOINT(3, 3));
    this->ren->copy(this->ren, scene->map_tex, NULL);
    OMG_LdtkLevel* level = &scene->map.levels.data[0];
    scene->jumper_src.x = (float)scene->anim.cur_frame * 32;
    for (size_t i = 0; i < level->layers.len; i++) {
        OMG_LdtkLayer* lay = &level->layers.data[i];
        if (!lay->is_entity_layer)
            continue;
        scene->jumper_src.y = 0.0f;
        for (size_t j = 0; j < lay->entities.len; j++) {
            OMG_LdtkEntity* ent = &lay->entities.data[j];
            scene->jumper_src.y = (scene->jumper_src.y == 0.0f) ? 32.0f : 0.0f;
            this->ren->copy_ex(
                this->ren, this->jumper_anims,
                &scene->jumper_src, &ent->rect, NULL, 0.0
            );
        }
    }
    this->ren->set_scale(this->ren, NULL, &OMG_FPOINT(1, 1));
    for (size_t i = 0; i < MAX_OBJECTS; i++) {
        OMG_Object* obj = scene->objects[i];
        if (OMG_ISNULL(obj))
            continue;
        if (OMG_ISNOTNULL(obj->on_paint))
            obj->on_paint(obj, this->sm->cur_scene);
    }
    OMG_FPoint circle_pos;
    circle_pos.y = 400.0f + this->omg->std->sinf((float)(scene->sin_timer.time * 4.0)) * 100.0f;
    if (scene->x_timer.time >= 5.0)
        circle_pos.x = 100.0f + 500.0f - (float)(scene->x_timer.time - 5.0) * 100.0f;
    else
        circle_pos.x = 100.0f + (float)scene->x_timer.time * 100.0f;
    this->ren->fill_circle(this->ren, &circle_pos, 50.0f, &scene->circle_color);
    // this->ren->copy(this->ren, this->font_tex, &OMG_FPOINT(200, 200));
#if SUPPORT_FONT
    this->ren->font_render_to(this->ren, NULL, this->fps_font, &this->fps_str, NULL, &OMG_COLOR_MAKE_RGB(0, 255, 255), NULL);
#endif
    this->ren->set_scale(this->ren, NULL, &OMG_FPOINT(0.5, 0.5));
    omg_bmfont_render(&scene->bmfont, &this->fps_str, &OMG_FPOINT_MAKE(0, 200));
    this->ren->set_scale(this->ren, NULL, &OMG_FPOINT(1, 1));
    this->ren->flip(this->ren);
    // OMG_INFO(this->omg, "Scene paint");
    return false;
}

bool scene_on_run(TestScene* scene) {
    App* this = (App*)((OMG_Scene*)scene)->data;
    omg_obj_anim_run_state(&scene->anim, 0);
    this->omg->clock->reset(this->omg->clock);
    OMG_INFO(this->omg, "Scene run");
    return false;
}

bool scene_on_init(TestScene* scene) {
    App* this = (App*)((OMG_Scene*)scene)->data;
    OMG_BPO();
    scene_base->on_destroy = scene_on_destroy;
    scene_base->on_update = scene_on_update;
    scene_base->on_paint = scene_on_paint;
    scene_base->on_run = scene_on_run;
    OMG_EPO();
    this->omg->std->memset(scene->objects, 0, sizeof(scene->objects));
    scene->anim.data = &scene->anim_data;
    omg_anim_sprite_data_init(&scene->anim_data, this->omg);
    OMG_ARRAY_PUSH_DUMMY(&scene->anim_data.states);
    omg_anim_sprite_state_init(&scene->anim_data.states.data[0], this->omg, 0.2, 4);
    omg_obj_anim_sprite_init(&scene->anim);
    omg_obj_timer_init(&scene->timer, this->omg);
    scene->timer.duration = 1.0;
    scene->timer.running = true;
    omg_obj_anim_timer_init(&scene->sin_timer, this->omg);
    scene->sin_timer.duration = OMG_M_PI2;
    scene->sin_timer.running = true;
    omg_obj_anim_timer_init(&scene->x_timer, this->omg);
    scene->x_timer.duration = 10.0;
    scene->x_timer.running = true;
    OMG_BPO();
    scene->objects[0] = &scene->timer;
    scene->objects[1] = &scene->sin_timer;
    scene->objects[2] = &scene->x_timer;
    scene->objects[3] = &scene->anim;
    OMG_EPO();
    scene->circle_color = OMG_COLOR_MAKE_RGB(0, 0, 255);
    // Load Bitmap Font
    OMG_File* file = this->omg->file_from_fp(this->omg, NULL, &OMG_STR("assets/goldFont-uhd.fnt"), OMG_FILE_MODE_RT);
    size_t data_size = file->get_size(file);
    char* data = OMG_MALLOC(this->omg->mem, data_size + 10);
    this->omg->std->memset(&data[data_size], 0, 8);
    file->read(file, data, 1, data_size);
    omg_bmfont_init(&scene->bmfont, this->font_tex, this->ren, data, data_size);
    OMG_FREE(this->omg->mem, data);
    file->destroy(file);
    // Load Ldtk Map
    file = this->omg->file_from_fp(this->omg, NULL, &OMG_STR("assets/test_map.txt"), OMG_FILE_MODE_RT);
    data_size = file->get_size(file);
    data = OMG_MALLOC(this->omg->mem, data_size + 10);
    this->omg->std->memset(&data[data_size], 0, 8);
    file->read(file, data, 1, data_size);
    omg_ldtk_init(&scene->map, this->omg, data, data_size);
    OMG_FREE(this->omg->mem, data);
    file->destroy(file);
    // Pre-render map
    OMG_LdtkLevel* level = &scene->map.levels.data[0];
    scene->map_tex = this->ren->tex_create(this->ren, NULL, &OMG_FPOINT(level->rect.w, level->rect.h), OMG_TEXTURE_ACCESS_TARGET, false);
    this->ren->tex_set_scale_mode(this->ren, scene->map_tex, OMG_SCALE_MODE_NEAREST);
    this->ren->set_target(this->ren, scene->map_tex);
    OMG_FRect src;
    OMG_FRect dst;
    OMG_Texture* tex;
    for (size_t i = 0; i < level->layers.len; i++) {
        OMG_LdtkLayer* lay = &level->layers.data[i];
        if (lay->is_entity_layer)
            continue;
        tex = (i == 2) ? this->tilemap2 : this->tilemap1;
        src.w = src.h = dst.w = dst.h = lay->grid_size;
        dst.w = dst.h = lay->grid_size;
        for (size_t j = 0; j < lay->tiles.len; j++) {
            OMG_LdtkTile* tile = &lay->tiles.data[j];
            // if ((i != 2) && tile->id <= 0)
            //     continue;
            src.x = tile->src.x;
            src.y = tile->src.y;
            src.w = src.h = lay->grid_size;
            dst.x = tile->pos.x;
            dst.y = tile->pos.y;
            if (tile->flip_x)
                src.w = -src.w;
            if (tile->flip_y)
                src.h = -src.h;
            this->ren->copy_ex(
                this->ren, tex,
                &src, &dst, NULL, 0.0
            );
        }
    }
    scene->jumper_src.x = scene->jumper_src.y = 0.0f;
    scene->jumper_src.w = scene->jumper_src.h = 32.0f;
    this->ren->set_target(this->ren, NULL);
    this->omg->set_text_input_state(this->omg, OMG_TEXT_INPUT_ENABLED);
    OMG_INFO(this->omg, "Scene init");
    return false;
}

void app_on_update(OMG_EventUpdate* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    OMG_UNUSED(this);
    // this->audio->update(this->audio);
}

void app_on_paint(OMG_EventPaint* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    OMG_UNUSED(this);
}

void app_on_expose(OMG_EventExpose* event) {
    // app_on_update((OMG_EventUpdate*)event);
    app_on_paint((OMG_EventPaint*)event);
}

void app_on_key_down(OMG_EventKeyboard* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    if (event->code == OMG_SCANCODE_Q) {
        this->omg->auto_loop_stop(this->omg);
        return;
    }
    else if (event->code == OMG_SCANCODE_V)
        this->ren->set_vsync(this->ren, !this->win->vsync);
    else if (event->code == OMG_SCANCODE_E)
        this->ren->aa = !this->ren->aa;
    else if (event->code == OMG_SCANCODE_T)
        this->fps_font->aa = !this->fps_font->aa;
    else if (event->code == OMG_SCANCODE_L)
        this->win->set_window_mode(
            this->win,
            (this->win->window_mode != OMG_WIN_MODE_WINDOW) ? OMG_WIN_MODE_WINDOW : OMG_WIN_MODE_DESKTOP_FULLSCREEN
        );
    else if (event->code == OMG_SCANCODE_P)
        this->win->mouse_set_system_cursor(this->win, OMG_SYSTEM_CURSOR_CROSSHAIR);
    else if (event->code == OMG_SCANCODE_LEFTBRACKET)
        this->win->mouse_set_system_cursor(this->win, OMG_SYSTEM_CURSOR_ARROW);
    else if (event->is_pressed) {
        if (event->code == OMG_SCANCODE_U)
            this->win->mouse_set_rel(this->win, 2); // 0 - Off, 1 - On, 2 - Toggle, Other - Query
        else if (event->code == OMG_SCANCODE_I)
            this->win->set_grab(this->win, 2);
        else if (event->code == OMG_SCANCODE_O)
            this->win->cursor_set_shown(this->win, 2);
        else if (event->code == OMG_SCANCODE_B)
            this->win->set_opacity(this->win, this->win->get_opacity(this->win) > 0.5f ? 0.5f : 1.0f);
    }
}

void app_on_mouse_move(OMG_EventMouseMove* event) {
    App* this = OMG_ARG_FROM_EVENT(event);
    if (!(event->state & OMG_MBUTTON_LEFT))
        return;
    this->ren->set_scale(this->ren, &OMG_FPOINT_MAKE(
        this->ren->offset.x + event->rel.x, this->ren->offset.y + event->rel.y
    ), NULL);
}

void app_init(App* this, OMG_EntryData* data) {
    this->exit_code = 1;
#if 0
#elif OMG_SUPPORT_WIN && !OMG_IS_UWP
    this->omg = omg_create_by_type(data, OMG_OMEGA_TYPE_WIN);
#elif OMG_SUPPORT_SDL2
    this->omg = omg_create_by_type(data, OMG_OMEGA_TYPE_SDL2);
#elif OMG_SUPPORT_RAYLIB
    this->omg = omg_create_by_type(data, OMG_OMEGA_TYPE_RAYLIB);
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
        OMG_ERROR(this->omg, "OMG Init Fail");
        this->omg->destroy(this->omg);
        return;
    }
    this->win->resizable = true;
#if OMG_IS_EMSCRIPTEN
    this->win->size.w = 1024.0f;
    this->win->size.h = 768.0f;
#endif
    if (this->win->default_init(this->win)) {
        OMG_ERROR(this->omg, "OMG Window Init Fail");
        this->omg->destroy(this->omg);
        return;
    }
    if (
        this->omg->winmgr->image_loader_alloc(this->omg->winmgr) ||
        this->omg->winmgr->img->init(this->omg->winmgr->img)
    ) {
        OMG_ERROR(this->omg, "OMG Image Loader Init Fail");
        this->omg->destroy(this->omg);
        return;
    }
    if (this->win->type == OMG_WIN_TYPE_WIN && 0)
        this->win->ren_type = OMG_REN_TYPE_WIN;
    OMG_INFO(this->omg, "Scene init");
    OMG_String temp_env = this->omg->env_get(this->omg, &OMG_STR("OMG_SOFTWARE_RENDERER"));
    int force_ren_driver = OMG_REN_DRIVER_AUTO;
    if (OMG_IS_WIN)
        force_ren_driver = (this->omg->type == OMG_OMEGA_TYPE_WIN) ? OMG_REN_DRIVER_D3D11 : OMG_REN_DRIVER_OPENGL;
    if (temp_env.type >= 0)
        force_ren_driver = OMG_REN_DRIVER_SOFTWARE;
    // force_ren_driver = OMG_REN_DRIVER_D3D11;
    omg_string_destroy(&temp_env);
    if (
        this->win->renderer_alloc(this->win) ||
        !(this->win->ren->driver = force_ren_driver) ||
        this->win->ren->init(this->win->ren)
    ) {
        OMG_ERROR(this->omg, "OMG Renderer Init Fail");
        this->omg->destroy(this->omg);
        return;
    }
    if ((!OMG_IS_EMSCRIPTEN && !OMG_IS_ANDROID && 0) || this->omg->type == OMG_OMEGA_TYPE_WIN)
        this->omg->audio_type = OMG_AUDIO_TYPE_FMOD;
    if (sizeof(void*) == 4 && this->omg->audio_type == OMG_AUDIO_TYPE_FMOD)
        this->omg->audio_type = OMG_AUDIO_TYPE_SDL2;
    /* if (this->omg->audio_alloc(this->omg) || this->omg->audio->init(this->omg->audio)) {
        OMG_ERROR(this->omg, "OMG Audio Init Fail");
        this->omg->destroy(this->omg);
        return;
    } */
#if SUPPORT_FONT
    if (this->omg->winmgr->fontmgr_alloc(this->omg->winmgr) || this->omg->winmgr->fnt->init(this->omg->winmgr->fnt)) {
        OMG_ERROR(this->omg, "OMG Font Init Fail");
        this->omg->destroy(this->omg);
        return;
    }
#endif
    this->fnt = this->omg->winmgr->fnt;
    this->audio = this->omg->audio;
    this->ren = this->win->ren;
    this->ren->soft_offset = false;
    this->ren->aa = !OMG_IS_EMSCRIPTEN;
#if SUPPORT_FONT
    this->fps_font = this->fnt->font_from_fp(this->fnt, NULL, &OMG_STR("assets/segoeuib.ttf"), -1, 32.0f);
    this->fps_font->wrapping = false;
    this->fps_font->text_type = OMG_FONT_TEXT_TYPE_TEXT;
#endif
    this->omg->std->memcpy(this->fps_buf, "FPS:               \0", 20);
    this->fps_str = OMG_STRING_MAKE_BUFFER_A(this->fps_buf);
    this->clock = this->omg->clock;
    this->win->allow_alt = false;
    this->omg->event_arg = this;
    this->omg->on_update = app_on_update;
    this->omg->on_paint = app_on_paint;
    this->omg->on_expose = app_on_expose;
    this->omg->on_loop_stop = app_on_destroy;
    this->omg->on_key_down = app_on_key_down;
    this->omg->on_mouse_move = app_on_mouse_move;
    this->font_tex = OMG_REN_TEXTURE_FROM_FILE(this->ren, &OMG_STR("assets/goldFont-uhd.png"));
    this->tilemap1 = OMG_REN_TEXTURE_FROM_FILE(this->ren, &OMG_STR("assets/Cavernas_by_Adam_Saltsman.png"));
    this->tilemap2 = OMG_REN_TEXTURE_FROM_FILE(this->ren, &OMG_STR("assets/SunnyLand_by_Ansimuz-extended.png"));
    this->jumper_anims = OMG_REN_TEXTURE_FROM_FILE(this->ren, &OMG_STR("assets/jumper.png"));
    this->ren->tex_set_scale_mode(this->ren, this->font_tex, OMG_SCALE_MODE_LINEAR);
    this->ren->tex_set_scale_mode(this->ren, this->tilemap1, OMG_SCALE_MODE_NEAREST);
    this->win->set_min_size(this->win, &OMG_FPOINT(320, 200));
    temp_env = this->omg->env_get(this->omg, &OMG_STR("OMG_MS_CLOCK"));
    this->clock->init(this->clock, !(temp_env.type >= 0));
    omg_string_destroy(&temp_env);
    this->clock->wait_for_limit = false;
    // this->clock->set_fps_limit(this->clock, 400.0);
    this->win->set_title(this->win, &OMG_STR("Test Window"));
    this->sm = OMG_MALLOC(this->omg->mem, sizeof(OMG_SceneMgr));
    this->sc = OMG_MALLOC(this->omg->mem, sizeof(TestScene));
    omg_scenemgr_init(this->sm, this->ren);
    omg_scenemgr_scene_fill(this->sm, this->sc);
    OMG_BPO();
    scene_base->on_init = scene_on_init;
    OMG_EPO();
    omg_scenemgr_scene_init(this->sm, this->sc, this);
    OMG_INFO(this->omg, "Hello world ", 1337.228f, " ", 228.1337, " 1", 228, "1 0x", (void*)this->omg);
    this->win->show(this->win, true);
    this->win->mouse_set_system_cursor(this->win, OMG_SYSTEM_CURSOR_DEFAULT);
    omg_scenemgr_scene_run(this->sm, this->sc);
    OMG_String cwd = this->omg->get_cwd(this->omg, true);
    OMG_INFO(this->omg, "Current path: ", &cwd);
    omg_string_destroy(&cwd);
    // this->clock->reset(this->clock);
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
