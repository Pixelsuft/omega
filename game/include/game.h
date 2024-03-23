#pragma once
#include <omega/omega.h>
#include <omega/scene.h>
#include <omega/scene_objects.h>

typedef struct {
    OMG_Scene parent;
} GameScene;

bool game_scene_init(GameScene* this);
