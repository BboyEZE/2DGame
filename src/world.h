
#pragma once
#include <SDL2/SDL.h>

#include "camera.h"
#include "player.h"
#include "collision.h"

#define PROP_COUNT 64



typedef struct Tree {
    int xPos;
    int yPos;
    Body body;
} Tree;

typedef struct BorderWall {
    int x;
    int y;
    Body body;
} BorderWall;

typedef struct World {
    int width;
    int height;
    //prop information
    Tree *props;
    int propCount;
    //border information
    BorderWall *collider;
    int colliderCount;
    //world tiles information
    unsigned char *tiles;
    int tileCols;
    int tileRows;
} World;


void clampPlayerToWorld(Player *p,World *w);
void destoryForest(World *w);

void renderWorld(World *w, Camera *c, SDL_Renderer* renderer);

int generateForest(World *w, int count, SDL_Renderer* renderer);

bool projectileIntersection(int left, int right, int top, int bot, World * w);

extern World forest;
