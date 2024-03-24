#include <game.h>
#include <menu.h>
#include <aamain.h>
#define base ((OMG_Scene*)this)
#define rn app->ren

bool game_scene_on_update(GameScene* this) {
    App* app = base->data;
    app->clock->update(app->clock);
    base->dt = app->clock->dt;
    this->p.parent.rect.x = this->p.r.x - 9.0f;
    this->p.parent.rect.y = this->p.r.y - 11.0f;
    OMG_BEGIN_POINTER_CAST();
    this->p.a.parent.on_update(&this->p.a, this);
    OMG_END_POINTER_CAST();
    return false;
}

bool game_scene_on_paint(GameScene* this) {
    App* app = base->data;
    rn->begin(rn);
    rn->clear(rn, &this->ldtk->levels.data[0].bg_color);
    rn->set_scale(rn, NULL, &app->sc);
    rn->copy(rn, this->bg[0], NULL);
    OMG_FRect p_src;
    p_src.w = p_src.h = 32.0f;
    p_src.x = (float)(this->p.a.cur_frame * 32);
    p_src.y = (float)(this->p.a.cur_state * 32);
    rn->copy_ex(rn, app->ld.tex[3], &p_src, &this->p.parent.rect, NULL, 0.0);
    rn->set_scale(rn, NULL, &OMG_FPOINT(app->sc.w / 3.0f, app->sc.h / 3.0f));
    app_draw_fps(app);
    rn->set_scale(rn, NULL, &app->sc);
    rn->flip(rn);
    return false;
}

bool game_scene_on_run(GameScene* this) {
    App* app = base->data;
    omg_obj_anim_run_state(&this->p.a, 0);
    app->clock->reset(app->clock);
    return false;
}

void game_scene_on_resize(GameScene* this, OMG_EventResize* event) {
    App* app = base->data;
    OMG_UNUSED(event);
    rn->set_scale(rn, NULL, &app->sc);
}

void game_scene_on_keyboard(GameScene* this, OMG_EventKeyboard* event) {
    App* app = base->data;
    if (IS_EXIT_CODE(event->code)) {
        omg_scenemgr_scene_destroy(&app->sm, this);
        app->omg->auto_loop_stop(app->omg);
    }
    else if (IS_BACK_CODE(event->code)) {
        this->should_back = true;
        omg_scenemgr_scene_destroy(&app->sm, this);
    }
}

bool game_scene_on_destroy(GameScene* this) {
    App* app = base->data;
    rn->tex_destroy(rn, this->bg[0]);
    omg_anim_sprite_data_destroy(this->p.a.data, app->omg, true);
    return false;
}

bool game_scene_on_stop(GameScene* this) {
    App* app = base->data;
    if (!this->should_back)
        return false;
    MenuScene* scene = OMG_MALLOC(app->omg->mem, sizeof(MenuScene));
    omg_scenemgr_scene_fill(&app->sm, scene);
    OMG_BEGIN_POINTER_CAST();
    scene->parent.on_init = menu_scene_init;
    OMG_END_POINTER_CAST();
    omg_scenemgr_scene_init(&app->sm, scene, app);
    omg_scenemgr_scene_run(&app->sm, scene);
    return false;
}

bool game_scene_init(GameScene* this) {
    App* app = base->data;
    this->p.parent.rect.w = this->p.parent.rect.h = 32.0f;
    this->p.a.data = &this->p.d;
    this->p.r.w = 14.0f;
    this->p.r.h = 21.0f;
    this->p.r.x = this->p.r.y = 100.0f;
    omg_anim_sprite_data_init(this->p.a.data, app->omg);
    OMG_ARRAY_SET_LEN(&this->p.d.states, 5, false);
    omg_anim_sprite_state_init(&this->p.d.states.data[0], app->omg, 0.4, 4); // Idle
    omg_anim_sprite_state_init(&this->p.d.states.data[1], app->omg, 0.4, 2); // Fall
    omg_anim_sprite_state_init(&this->p.d.states.data[2], app->omg, 0.4, 6); // Classical run
    omg_anim_sprite_state_init(&this->p.d.states.data[3], app->omg, 0.4, 4); // Jump
    omg_anim_sprite_state_init(&this->p.d.states.data[4], app->omg, 0.4, 4); // Run
    omg_obj_anim_sprite_init(&this->p.a);
    app->sc.w = app->win->size.w / 800.0f;
    app->sc.h = app->win->size.h / 600.0f;
    this->ldtk = &app->ld.mp[0];
    this->bg[0] = rn->tex_create(rn, NULL, &OMG_FPOINT(800, 600), OMG_TEXTURE_ACCESS_TARGET, true);
    rn->set_scale(rn, NULL, &OMG_FPOINT(1, 1));
    rn->set_target(rn, this->bg[0]);
    rn->clear(rn, &OMG_RGBA(0, 0, 0, 0));
    for (size_t li = 0; li < this->ldtk->levels.data[0].layers.len; li++) {
        OMG_LdtkLayer* lay = &this->ldtk->levels.data[0].layers.data[li];
        if (!lay->is_entity_layer) {
            for (size_t i = 0; i < lay->tiles.len; i++) {
                OMG_LdtkTile* tile = &lay->tiles.data[i];
                rn->copy_ex(
                    rn, app->ld.tex[1], &OMG_FRECT(tile->src.x, tile->src.y, 32, 32),
                    &OMG_FRECT(tile->pos.x, tile->pos.y, 32, 32), NULL, 0.0
                );
            }
        }
    }
    rn->set_target(rn, NULL);
    rn->set_scale(rn, NULL, &app->sc);
    OMG_BEGIN_POINTER_CAST();
    base->on_run = game_scene_on_run;
    base->on_update = game_scene_on_update;
    base->on_paint = game_scene_on_paint;
    base->on_resize = game_scene_on_resize;
    base->on_key_down = game_scene_on_keyboard;
    base->on_destroy = game_scene_on_destroy;
    base->on_stop = game_scene_on_stop;
    OMG_END_POINTER_CAST();
    this->should_back = false;
    base->inited = true;
    base->was_allocated = true;
    return false;
}
