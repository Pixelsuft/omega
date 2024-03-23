#pragma once
#include <omega/omega.h>
#include <omega/scene.h>
#include <omega/scene_objects.h>

typedef struct {
    OMG_Scene parent;
    double logo_timer;
    OMG_Texture* logo;
    bool should_cont;
} LogoScene;

bool logo_scene_init(LogoScene* this);
