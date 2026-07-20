

#pragma once
#include <SDL2/SDL.h>
#include "player.h"
#include <stdbool.h>

typedef struct Projectile{
    float xPos;
    float yPos;
    int speed;
    float xAimDirection;
    float yAimDirection;
    float lifetime;
    SDL_Rect body;

} Projectile;

bool moveProjectile(Projectile *p, float deltaTime, int widthMax, int heightMax, int speed);

void getDeltaMousePosition(Player* player, int mouseX, int mouseY, Projectile* p);
