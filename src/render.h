#pragma once
#include "collision.h"
#include "projectile.h"

typedef struct RenderObj{
    Body *b;
    int kind;
    int maxY;

} RenderObj;

int initVideo();
int initGameWindow();
int initRenderer();
int makePlayerRender();

void setCameraPositionForRender(int x, int y);

void clearRenderer();
void tearDownAllRender(char jump);

int createTileTextures();
int createForestTextures();

void renderProjectile(Projectile *p);

void renderPlayer(Body *b);

void renderTile(unsigned char type, SDL_Rect *pos);
void renderTree(Body *b);
void renderBorder(Body *b);

void initWindowScaling();

void destroyForestTextures();
void destoryTileTextures();

void setMouseVariableToMousePosition(int *mouseX, int *mouseY, float *logicalMouseX, float *logicalMouseY);

void addObjToList(int kind, Body *body);
int renderScreen();