

#pragma once
#include <SDL2/SDL.h>
#include "player.h"
#include <stdbool.h>

typedef struct Projectile{
    float xPos;
    float yPos;
    int speed;
    int xAimDirection;
    int yAimDirection;
    float lifetime;
    SDL_Rect body;

} Projectile;

bool moveProjectile(Projectile *p, float deltaTime);

void getDeltaMousePosition(Player* player, int mouseX, int mouseY, Projectile* p);
