#pragma once
#include <omega/omega.h>
#include <omega/scene.h>
#include <omega/scene_objects.h>

typedef struct {
    OMG_Scene parent;
} MenuScene;

bool menu_scene_init(MenuScene* this);
