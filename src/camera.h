
#pragma once

#include "player.h"

typedef struct Camera {
    float x;
    float y;
    int window_width; // adjusted window width 
    int window_height; // adjusted window height
} Camera;

void moveCamera(Camera* camera, Player* player);

Vec2 cameraAdjustWorld(Camera* camera, const int WORLD_MAX_WIDTH, const int WOLRD_MAX_HEIGHT);