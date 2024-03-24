#pragma once
#include <omega/omega.h>
#include <omega/scene.h>
#include <omega/scene_objects.h>
#include <omega/ldtk.h>
#define P_A_IDLE 0
#define P_A_FALL 1
#define P_A_CRUN 2
#define P_A_JUMP 3
#define P_A_RUN 4

/*
Sprite anim info:
0) idle
1) fall
2) cassical run
3) jump
4) run
*/

typedef struct {
    OMG_Object parent;
    OMG_ObjectAnimSprite a;
    OMG_AnimSpriteData d;
    OMG_FRect r; // Hitbox: [9, 11, 14, 21]
    OMG_LdtkEntity* gr_o;
    float x_speed;
    float y_speed;
    int dir;
    bool face_left;
    bool on_ground;
} Player;
