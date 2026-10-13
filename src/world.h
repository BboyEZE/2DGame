
#pragma once
#include <SDL2/SDL.h>

#include "camera.h"
#include "player.h"
#include "collision.h"

#define PROP_COUNT 64



typedef struct Tree {
    Body body;
    Body collisionBox;
} Tree;

typedef struct BorderWall {
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

void renderWorld(World *w, Player *p, Camera *c);

int generateForest(World *w, int count);

bool projectileIntersection(Body *projBody, World * w);

extern World forest;
