#pragma once
#include <omega/omega.h>
#include <omega/scene.h>
#include <omega/scene_objects.h>

typedef struct {
    OMG_Scene parent;
    OMG_Texture* bg;
    OMG_ObjectAnimTimer sc_t1;
} MenuScene;

bool menu_scene_init(MenuScene* this);
