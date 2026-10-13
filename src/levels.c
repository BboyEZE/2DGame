#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>

#include "world.h"
#include "camera.h"
#include "player.h"
#include "collision.h"
#include "render.h" 

#define TILE_SIZE 16

#define TREE_W 20
#define TREE_H 40

#define COLLIDER_THICKNESS 32
#define COLLIDER_OFFSET 12

#define BORDER_SHOWING_AMT 4

#define LAKE_ROW_START 28
#define LAKE_COL_START 32
#define LAKE_WIDTH 30
#define LAKE_HEIGHT 13

void clampPlayerToWorld(Player *p, World *w){
    int playerX = p->body.x;
    int playerY = p->body.y;
    int playerMaxX = p->body.x + p->body.w;
    int playerMaxY = p->body.y + p->body.h;

    if(playerX <= w->collider[0].body.x + w->collider[0].body.w) { p->body.x =  w->collider[0].body.x + w->collider[0].body.w + 1; }
    if(playerY <= w->collider[1].body.y + w->collider[1].body.h) { p->body.y =  w->collider[1].body.y + w->collider[1].body.h + 1; }
    if(playerMaxX >= w->collider[2].body.x) { p->body.x =  w->collider[2].body.x - 1 - p->body.w; }
    if(playerMaxY >= w->collider[3].body.y) { p->body.y =  w->collider[3].body.y - 1 - p->body.h; }
}

bool projectileIntersection(Body *projBody, World * w){
    for(int i = 0; i<w->propCount; i++){
        if(intersecting(projBody, &w->props[i].body)) return true;
    }
    for(int i=0; i<w->colliderCount; i++){
        if(intersecting(projBody, &w->collider[i].body)) return true;
    }
    return false;
}

void renderWorld(World *w, Player *p, Camera *c){
    
    //ground
    int startCol = c->x / TILE_SIZE;
    int endCol = 1 + (c->x + c->window_width) / TILE_SIZE;
    int startRow = c->y / TILE_SIZE;
    int endRow = 1 + (c->y + c->window_height) / TILE_SIZE;
    SDL_Rect tilePos = { .w = TILE_SIZE, .h = TILE_SIZE };
    for(int row = startRow; row < endRow; row++){
        for(int col = startCol; col < endCol; col++){
            unsigned char type = w->tiles[row * w->tileCols + col];
            tilePos.x = col * TILE_SIZE - c->x;
            tilePos.y = row * TILE_SIZE - c->y;
            renderTile(type, &tilePos);
        }
    }
    

    //trees
    for(int i = 0; i< w->propCount; i++){
        int propX = w->props[i].body.x - c->x;
        int propY = w->props[i].body.y - c->y;
        int propXMax = propX + w->props[i].body.w;
        int propYMax = propY + w->props[i].body.h;
        if( (propXMax > 0) && !(propX > c->window_width) && (propYMax > 0 ) && !(propY > c->window_height)){
            addObjToList(1, &w->props[i].body);
        }
    }

    //colliders
    for(int i = 0; i< w->colliderCount; i++){
        int wallX = w->collider[i].body.x - c->x;
        int wallY = w->collider[i].body.y - c->y;
        int wallXMax = wallX + w->collider[i].body.w;
        int wallYMax = wallY + w->collider[i].body.h;
        if ((wallXMax > 0) && !(wallX > c->window_width) && (wallYMax > 0) && !(wallY > c->window_height)){
            renderBorder(&w->collider[i].body);
        }
    }  

    addObjToList(0 , &p->body);
}

World forest = { 
    .width=1600,
    .height=1000,
};

int generateForest(World *w, int count){
    int collider = 4;

    if(createForestTextures() == 1) { return 1; }

    //prop init (trees)
    w->propCount = count;
    w->props = malloc(count * sizeof(Tree));
    if(w->props==NULL){
        fprintf(stderr, "ERROR: malloc function for the props did NOT work");
    }
    // putting each tree into the array
    for(int i = 0; i < count; i++){
        int x = rand() % (w->width - TREE_W);
        int y = rand() % (w->height - TREE_H);
        w->props[i] = (Tree) { .body = {.x = x, .y = y, .w = TREE_W, .h = TREE_H }, .collisionBox = { .w=TREE_W, .h=TREE_H/4, .x= x, .y= (3)*(TREE_H/4) } };
    }
    
    //tiles init
    w->tileCols = ceil((float)w->width / TILE_SIZE);
    if(w->tileCols == 0) { return 1; }
    w->tileRows = ceil((float)w->height / TILE_SIZE);
    if(w->tileRows == 0) { return 1; }

    w->tiles = malloc(w->tileCols * w->tileRows * sizeof(unsigned char));
    for(int i = 0; i < w->tileCols * w->tileRows; i++) {
        w->tiles[i] = 0;                                                                                                                                                                                    
    }
    
    for(int row = 0; row < LAKE_HEIGHT; row++){
        for(int col = 0; col < LAKE_WIDTH; col++){
            w->tiles[(row + LAKE_ROW_START) * w->tileCols + (col + LAKE_COL_START)] = 1;
        }
    }
    //collider init
    w->colliderCount = collider; 
    w->collider = malloc(collider * sizeof(BorderWall));

    w->collider[0] = (BorderWall) { .body = (Body){ .x = COLLIDER_OFFSET-COLLIDER_THICKNESS, .y = 0, .w = COLLIDER_THICKNESS, .h = w->height, }};
    w->collider[1] = (BorderWall) { .body = (Body){ .x = 0, .y = COLLIDER_OFFSET-COLLIDER_THICKNESS, .w = w->width, .h = COLLIDER_THICKNESS }};
    w->collider[2] = (BorderWall) { .body = (Body){ .x = w->width - COLLIDER_OFFSET, .y = 0, .w = COLLIDER_THICKNESS, .h = w->height }};
    w->collider[3] = (BorderWall) { .body = (Body){ .x = 0, .y = w->height -COLLIDER_OFFSET, .w = w->width, .h = COLLIDER_THICKNESS }};
    return 0;
}

void destoryForest(World *w){
    free(w->props);
    w->props = NULL;
    free(w->collider);
    w->collider=NULL;   
    destroyForestTextures();
}