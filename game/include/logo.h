#pragma once
#include <omega/omega.h>
#include <omega/scene.h>

typedef struct {
    OMG_Scene parent;
    OMG_Texture* logo;
} LogoScene;

bool logo_scene_init(LogoScene* this);
