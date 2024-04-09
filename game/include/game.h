#pragma once
#include <omega/omega.h>
#include <omega/scene.h>
#include <omega/ldtk.h>
#include <omega/scene_objects.h>
#include <player.h>

// Game class
typedef struct {
    OMG_Scene parent;
    Player p;
    OMG_FPoint offset;
    OMG_Texture* bg[1];
    OMG_Ldtk* ldtk;
    int64_t fingers[2];
    float cloud_offset;
    bool should_back;
    bool render_hitbox;
} GameScene;

bool game_scene_init(GameScene* this);
