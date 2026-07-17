

#pragma once
#include "player.h"
#include "projectile.h"

typedef struct Vec2 {
    float x;
    float y;
} Vec2;

Vec2 getProjectileDeltaDistance(float playerX, float playerY, int mouseX, int mouseY);