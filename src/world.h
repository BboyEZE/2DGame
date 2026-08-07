
#pragma once
#include <SDL2/SDL.h>

#include "camera.h"
#include "player.h"

#define PROP_COUNT 64

typedef struct Tree {
    int xPos;
    int yPos;
    SDL_Rect body;
} Tree;

typedef struct World {
    int width;
    int height;
    Tree *props;
    int propCount;
    SDL_Rect *collider;
    int colliderCount;
} World;

void renderWorld(World *w, Camera *c, SDL_Renderer* renderer);
void clampPlayerToWorld(Player *p,World *w);

void generateForest(World *w, int count);
void destoryForest(World *w);

extern World forest;
