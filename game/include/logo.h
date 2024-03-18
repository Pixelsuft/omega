#pragma once
#include <omega/omega.h>
#include <omega/scene.h>

typedef struct {
    OMG_Scene parent;
} LogoScene;

bool logo_scene_init(LogoScene* this);
