#pragma once
#include <omega/omega.h>
#include <omega/scene.h>
#include <omega/scene_objects.h>

// Menu class
typedef struct {
    OMG_Scene parent;
    OMG_Texture* bg;
    OMG_ObjectAnimTimer sc_t1;
    OMG_ObjectAnimTimer sc_t2;
    bool should_cont;
} MenuScene;

bool menu_scene_init(MenuScene* this);
