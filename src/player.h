//Information for the player and all the things you can do to the player
//definition for the player

#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "collision.h"


typedef struct Player {
        Body body;
        int health;
        SDL_Texture *pBodyImage;
} Player;

void xDirectionMove(Player *p, bool positive, float delta);
void yDirectionMove(Player *p, bool positive, float delta);
void xyDirectionMove(Player *p, bool posX, bool posY);

void healthIncrease(Player *p, int h);
bool healthDecrease(Player *p, int h);
