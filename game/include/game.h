#pragma once
#include <omega/omega.h>
#include <omega/scene.h>
#include <omega/ldtk.h>
#include <omega/scene_objects.h>

typedef struct {
    OMG_Scene parent;
    OMG_Texture* bg[1];
    OMG_Ldtk* ldtk;
    bool should_back;
} GameScene;

bool game_scene_init(GameScene* this);
