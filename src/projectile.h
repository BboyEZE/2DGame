

#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "collision.h"
#include "player.h"


typedef struct Projectile{
    int speed;
    float xAimDirection;
    float yAimDirection;
    float lifetime;
    Body body;

} Projectile;

bool moveProjectile(Projectile *p, float deltaTime, int widthMax, int heightMax, int speed);

void getDeltaMousePosition(Player* player, int mouseX, int mouseY, Projectile* p);
