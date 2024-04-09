#include <game.h>
#include <menu.h>
#include <aamain.h>
#define base ((OMG_Scene*)this)
#define rn app->ren

bool game_scene_on_update(GameScene* this) {
    App* app = base->data;
    app->clock->update(app->clock);
    base->dt = app->clock->dt;
    // If we have very low fps, let's just slow down fps
    if (base->dt > 0.5)
        base->dt = 0.5;
    // Update audio system
    app->au->update(app->au);
    // Update cloud pos
    this->cloud_offset -= (float)(base->dt * 25.0);
    if (this->cloud_offset <= -960.0f)
        this->cloud_offset += 960.0f;
    // Player physics. I haven't created platformes before, so code is very ugly
    this->p.parent.rect.x = this->p.r.x - 9.0f;
    this->p.parent.rect.y = this->p.r.y - 11.0f;
    float prev_y_speed = this->p.y_speed;
    this->p.y_speed += (float)base->dt * 750.0f;
    if (this->p.y_speed > 300.0f)
        this->p.y_speed = 300.0f;
    if (!this->p.on_ground)
        this->p.r.y += (this->p.y_speed + prev_y_speed) * (float)base->dt / 2.0f;
    this->p.r.x += (float)this->p.dir * this->p.x_speed * (float)base->dt;
    OMG_FPoint p_r;
    p_r.x = this->p.r.x + this->p.r.w;
    p_r.y = this->p.r.y + this->p.r.h;
    bool has_col = false;
    // Collision check with hitbox objects
    for (size_t li = 0; li < this->ldtk->levels.data[0].layers.len; li++) {
        OMG_LdtkLayer* lay = &this->ldtk->levels.data[0].layers.data[li];
        if (lay->is_entity_layer) {
            for (size_t i = 0; i < lay->entities.len; i++) {
                OMG_LdtkEntity* en = &lay->entities.data[i];
                if (en->id == 4) {
                    // Collision rect
                    if (p_r.x >= en->rect.x && this->p.r.x < (en->rect.x + en->rect.w)) {
                        if (p_r.y >= en->rect.y && this->p.r.y < (en->rect.y + en->rect.h)) {
                            has_col = true;
                            OMG_FRect cr;
                            omg_frect_intersect(&this->p.r, &en->rect, &cr);
                            cr.h /= this->p.y_speed;
                            cr.w /= this->p.x_speed * (float)this->p.dir;
                            // Collides from up
                            // bool is_dif = app->omg->std->fabsf(this->p.r.y + this->p.r.h - en->rect.y) <= 4.0f;
                            bool is_dif = app->omg->std->fabsf(cr.h) <= app->omg->std->fabsf(cr.w);
                            /* if (!is_dif) {
                                if (!(this->p.r.x <= en->rect.x) && !(p_r.x >= (en->rect.x + en->rect.w)))
                                    is_dif = true;
                            } */
                            if (0) {}
                            else if (!this->p.on_ground && this->p.y_speed >= 0.0f && is_dif && this->p.r.y < en->rect.y) {
                                this->p.on_ground = true;
                                this->p.air_jump = true;
                                this->p.r.y = en->rect.y - this->p.r.h;
                                this->p.gr_o = en;
                                omg_obj_anim_run_state(&this->p.a, (this->p.dir == 0) ? P_A_IDLE : P_A_CRUN);
                            }
                            else if (this->p.r.x < en->rect.x && !is_dif) {
                                this->p.r.x = en->rect.x - this->p.r.w;
                            }
                            else if (p_r.x > en->rect.x && !is_dif) {
                                this->p.r.x = en->rect.x + en->rect.w;
                            }
                            else if (this->p.y_speed < 0.0f && !this->p.on_ground && (p_r.y > (en->rect.y + en->rect.h)) && is_dif) {
                                this->p.y_speed = 0.0f;
                                this->p.r.y = en->rect.y + en->rect.h;
                            }
                        }
                    }
                }
            }
        }
    }
    if (this->p.on_ground && !has_col) {
        this->p.on_ground = false;
        this->p.y_speed = 0.001f;
        this->p.gr_o = NULL;
    }
    if (!this->p.on_ground && this->p.y_speed > 0.0f && (this->p.a.cur_state != P_A_FALL)) {
        omg_obj_anim_run_state(&this->p.a, P_A_FALL);
    }
    OMG_BEGIN_POINTER_CAST();
    // Update sprite anim cflass
    this->p.a.parent.on_update(&this->p.a, this);
    OMG_END_POINTER_CAST();
    return false;
}

bool game_scene_on_paint(GameScene* this) {
    App* app = base->data;
    rn->begin(rn);
    rn->set_scale(rn, &OMG_FPOINT(this->offset.x / 1.2f / app->sc.w, this->offset.y / 1.2f / app->sc.h), &OMG_FPOINT(app->sc.w * 1.2f, app->sc.h * 1.2f));
    rn->clear(rn, &this->ldtk->levels.data[0].bg_color);
    // Draw clouds
    OMG_FPoint cloud_pos;
    cloud_pos.y = 0.0f;
    cloud_pos.x = this->cloud_offset;
    rn->copy(rn, app->ld.tex[4], &cloud_pos);
    cloud_pos.x += 960.0f;
    rn->copy(rn, app->ld.tex[4], &cloud_pos);
    // Draw tilemap we rendered on init
    rn->set_scale(rn, &OMG_FPOINT(this->offset.x / app->sc.w, this->offset.y / app->sc.h), &app->sc);
    rn->copy(rn, this->bg[0], NULL);
    rn->copy(rn, app->ld.tex[2], &OMG_FPOINT(10, 544.0f - app->ld.tex[2]->size.h));
    if (this->render_hitbox) {
        // Draw hitboxes
        for (size_t li = 0; li < this->ldtk->levels.data[0].layers.len; li++) {
            OMG_LdtkLayer* lay = &this->ldtk->levels.data[0].layers.data[li];
            if (lay->is_entity_layer) {
                for (size_t i = 0; i < lay->entities.len; i++) {
                    OMG_LdtkEntity* en = &lay->entities.data[i];
                    if (en->id == 4) {
                        rn->fill_rect(rn, &en->rect, &OMG_RGBA(255, 0, 0, 100));
                        rn->draw_rect(rn, &en->rect, &OMG_RGBA(0, 255, 0, 255));
                    }
                }
            }
        }
    }
    // Draw player
    OMG_FRect p_src;
    p_src.h = 32.0f;
    p_src.w = this->p.face_left ? -32.0f : 32.0f;
    p_src.y = (float)(this->p.a.cur_state * 32); // Take frame from y pos by current anim state id (we can do this because we created it in special order)
    p_src.x = (float)(this->p.a.cur_frame * 32); // Take frame from x pos by current anim state frame
    rn->copy_ex(rn, app->ld.tex[3], &p_src, &this->p.parent.rect, NULL, 0.0);
    if (this->render_hitbox) {
        // Draw player hitbox
        OMG_FRect p_dst;
        p_dst.x = this->p.parent.rect.x + 9.0f;
        p_dst.y = this->p.parent.rect.y + 11.0f;
        p_dst.w = 14.0f;
        p_dst.h = 21.0f;
        rn->fill_rect(rn, &p_dst, &OMG_RGBA(0, 0, 255, 100));
        rn->draw_rect(rn, &p_dst, &OMG_RGBA(0, 255, 0, 255));
    }
    // Draw FPS
    rn->set_scale(rn, &OMG_FPOINT(this->offset.x / app->sc.w * 3.0f, this->offset.y / app->sc.h * 3.0f), &OMG_FPOINT(app->sc.w / 3.0f, app->sc.h / 3.0f));
    app_draw_fps(app);
    rn->set_scale(rn, &OMG_FPOINT(this->offset.x / app->sc.w, this->offset.y / app->sc.h), &app->sc);
    // Present
    rn->flip(rn);
    return false;
}

bool game_scene_on_run(GameScene* this) {
    App* app = base->data;
    // Reset game scene
    this->render_hitbox = false;
    app->au->mus_play(app->au, app->ld.mus[1], -1, 0.0, 0.2);
    this->p.r.w = 14.0f;
    this->p.r.h = 21.0f;
    this->p.r.x = this->p.r.y = 100.0f;
    this->p.y_speed = 0.0f;
    this->p.x_speed = 100.0f;
    this->p.on_ground = false;
    this->p.gr_o = NULL;
    this->p.dir = 0;
    this->p.face_left = false;
    this->p.air_jump = true;
    // Find player to set it's position
    for (size_t li = 0; li < this->ldtk->levels.data[0].layers.len; li++) {
        OMG_LdtkLayer* lay = &this->ldtk->levels.data[0].layers.data[li];
        if (lay->is_entity_layer) {
            for (size_t i = 0; i < lay->entities.len; i++) {
                OMG_LdtkEntity* en = &lay->entities.data[i];
                if (en->id == 6) {
                    // Player ID
                    this->p.r.x = en->rect.x;
                    this->p.r.y = en->rect.y;
                }
            }
        }
    }
    this->cloud_offset = 0.0f;
    this->should_back = false;
    app->win->cursor_set_shown(app->win, 0);
    app->clock->reset(app->clock);
    return false;
}

void game_scene_on_resize(GameScene* this, OMG_EventResize* event) {
    App* app = base->data;
    OMG_UNUSED(event);
    rn->set_scale(rn, NULL, &app->sc);
}

void game_player_do_jump(GameScene* this, bool high) {
    App* app = base->data;
    // Do jump if we can
    if (this->p.on_ground || this->p.air_jump) {
        app->au->snd_play(app->au, app->ld.snd[this->p.on_ground ? 0 : 1], 0, 0.0, 0.0);
        if (!this->p.on_ground)
            this->p.air_jump = false;
        this->p.on_ground = false;
        this->p.y_speed = high ? -400.0f : -275.0f;
        omg_obj_anim_run_state(&this->p.a, P_A_JUMP);
    }
}

void player_do_walk(GameScene* this, bool is_right, bool is_pressed) {
    // Start or stop walking
    if (is_right) {
        if (is_pressed) {
            this->p.dir = 1;
            this->p.face_left = false;
            if (this->p.on_ground) {
                omg_obj_anim_run_state(&this->p.a, P_A_CRUN);
            }
        }
        else {
            if (this->p.dir == 1) {
                this->p.dir = 0;
                if (this->p.on_ground) {
                    omg_obj_anim_run_state(&this->p.a, P_A_IDLE);
                }
            }
        }
    }
    else if (is_pressed) {
        this->p.dir = -1;
        this->p.face_left = true;
        if (this->p.on_ground) {
            omg_obj_anim_run_state(&this->p.a, P_A_CRUN);
        }        
    }
    else {
        if (this->p.dir == -1) {
            this->p.dir = 0;
            if (this->p.on_ground) {
                omg_obj_anim_run_state(&this->p.a, P_A_IDLE);
            }
        }
    }
}

void game_scene_on_keyboard(GameScene* this, OMG_EventKeyboard* event) {
    App* app = base->data;
    // Handle keyboard
    // I hate switch/case btw
    if (IS_EXIT_CODE(event->code) && !event->is_pressed) {
        omg_scenemgr_scene_destroy(&app->sm, this);
        app->omg->auto_loop_stop(app->omg);
    }
    else if (IS_BACK_CODE(event->code) && !event->is_pressed) {
        this->should_back = true;
        omg_scenemgr_scene_destroy(&app->sm, this);
    }
    else if ((event->code == OMG_SCANCODE_Z || event->code == OMG_SCANCODE_B || event->code == OMG_SCANCODE_SPACE) && event->is_pressed && !event->is_repeated) {
        game_player_do_jump(this, event->code == OMG_SCANCODE_B);
    }
    else if ((event->code == OMG_SCANCODE_LEFT || event->code == OMG_SCANCODE_A) && event->is_pressed && !event->is_repeated) {
        player_do_walk(this, false, true);
    }
    else if ((event->code == OMG_SCANCODE_RIGHT || event->code == OMG_SCANCODE_D) && event->is_pressed && !event->is_repeated) {
        player_do_walk(this, true, true);
    }
    else if ((event->code == OMG_SCANCODE_LEFT || event->code == OMG_SCANCODE_A) && !event->is_pressed) {
        player_do_walk(this, false, false);
    }
    else if ((event->code == OMG_SCANCODE_RIGHT || event->code == OMG_SCANCODE_D) && !event->is_pressed) {
        player_do_walk(this, true, false);
    }
    else if (event->code == OMG_SCANCODE_R && !event->is_pressed) {
        omg_scenemgr_scene_stop(&app->sm, this);
        omg_scenemgr_scene_run(&app->sm, this);
    }
    else if (event->code == OMG_SCANCODE_T && event->is_pressed) {
        app->clock->wait_for_limit = true;
        app->clock->set_fps_limit(app->clock, 15.0);
    }
    else if (event->code == OMG_SCANCODE_Y && event->is_pressed) {
        app->clock->set_fps_limit(app->clock, 0.0);
    }
    else if (event->code == OMG_SCANCODE_U && event->is_pressed) {
        app->clock->speed = 0.1;
        app->au->mus_set_speed(app->au, app->ld.mus[1], 0.1f);
    }
    else if (event->code == OMG_SCANCODE_I && event->is_pressed) {
        app->clock->speed = 1.0;
        app->au->mus_set_speed(app->au, app->ld.mus[1], 1.0f);
    }
    else if (event->code == OMG_SCANCODE_H && event->is_pressed) {
        this->render_hitbox = !this->render_hitbox;
    }
}

void game_scene_on_touch(GameScene* this, OMG_EventTouch* event) {
    // Handle touch
    if (event->moving) {
        if (event->finger_id == this->fingers[0]) {
            if (event->pos.x >= 0.25f && this->p.dir < 0) {
                player_do_walk(this, false, false);
                player_do_walk(this, true, true);
            }
            else if (event->pos.x < 0.25f && this->p.dir > 0) {
                player_do_walk(this, true, false);
                player_do_walk(this, false, true);
            }
        }
    }
    else if (event->pressed) {
        if (event->pos.x < 0.5f) {
            this->fingers[0] = event->finger_id;
            player_do_walk(this, event->pos.x >= 0.25f, true);
        }
        else {
            this->fingers[1] = event->finger_id;
            game_player_do_jump(this, event->pos.y < 0.2f);
        }
    }
    else {
        if (event->finger_id == this->fingers[0]) {
            bool is_r = this->p.dir == 1;
            player_do_walk(this, !is_r, false);
            player_do_walk(this, is_r, false);
            this->fingers[0] = -1337;
        }
        else if (event->finger_id == this->fingers[1]) {
            this->fingers[1] = -1337;
        }
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
    app->win->cursor_set_shown(app->win, 1);
    if (!this->should_back)
        return false;
    // We stopped, so let's go back to the menu
    app->au->mus_stop(app->au, app->ld.mus[1]);
    app->clock->speed = 1.0;
    app->au->mus_set_speed(app->au, app->ld.mus[1], 1.0f);
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
    app->au->mus_set_volume(app->au, app->ld.mus[1], 0.1f);
    app->au->snd_set_volume(app->au, app->ld.snd[0], 0.3f);
    app->au->snd_set_volume(app->au, app->ld.snd[1], 0.3f);
    this->p.parent.rect.w = this->p.parent.rect.h = 32.0f;
    // Init sprite anim in special order based on frames stored in texture
    this->p.a.data = &this->p.d;
    omg_anim_sprite_data_init(this->p.a.data, app->omg);
    OMG_ARRAY_SET_LEN(&this->p.d.states, 5, false);
    omg_anim_sprite_state_init(&this->p.d.states.data[P_A_IDLE], app->omg, 0.1, 4);
    omg_anim_sprite_state_init(&this->p.d.states.data[P_A_FALL], app->omg, 0.075, 2);
    omg_anim_sprite_state_init(&this->p.d.states.data[P_A_CRUN], app->omg, 0.075, 6);
    omg_anim_sprite_state_init(&this->p.d.states.data[P_A_JUMP], app->omg, 0.1, 4);
    omg_anim_sprite_state_init(&this->p.d.states.data[P_A_RUN], app->omg, 0.1, 4);
    this->p.d.states.data[P_A_JUMP].durations[3] = 100000.0f;
    omg_obj_anim_sprite_init(&this->p.a);
    omg_obj_anim_run_state(&this->p.a, P_A_FALL);
    this->offset.x = this->offset.y = 0.0f;
    // Should we keep aspect ratio
    if (app->sc.w == app->sc.h) {
        app->sc.w /= 800.0f / 640.0f;
        app->sc.h /= 600.0f / 480.0f;
        this->offset.x = (rn->size.w - 800.0f * app->sc.w) / 2.0f;
        this->offset.y = (rn->size.h - 600.0f * app->sc.h) / 2.0f;
    }
    else {
        app->sc.w = rn->size.w / 800.0f;
        app->sc.h = rn->size.h / 600.0f;
    }
    // Render tilemap into a texture we will create
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
    rn->set_scale(rn, &OMG_FPOINT(this->offset.x / app->sc.w, this->offset.y / app->sc.h), &app->sc);
    rn->tex_set_scale_mode(rn, app->ld.tex[4], OMG_SCALE_MODE_LINEAR);
    this->fingers[0] = this->fingers[1] = -1337;
    // Fill events
    OMG_BEGIN_POINTER_CAST();
    base->on_run = game_scene_on_run;
    base->on_update = game_scene_on_update;
    base->on_paint = game_scene_on_paint;
    base->on_resize = game_scene_on_resize;
    base->on_key_down = game_scene_on_keyboard;
    base->on_key_up = game_scene_on_keyboard;
    base->on_touch_down = game_scene_on_touch;
    base->on_touch_up = game_scene_on_touch;
    base->on_touch_move = game_scene_on_touch;
    base->on_destroy = game_scene_on_destroy;
    base->on_stop = game_scene_on_stop;
    OMG_END_POINTER_CAST();
    this->should_back = false;
    base->reset_input_touch = false;
    base->inited = true;
    base->was_allocated = true;
    return false;
}
