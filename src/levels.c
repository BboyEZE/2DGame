#include <SDL2/SDL.h>
#include <stdlib.h>

#include "world.h"
#include "camera.h"
#include "player.h"

#define TREE_W 20
#define TREE_H 50

#define COLLIDER_WIDTH 64
#define COLLIDER_HEIGHT 64

#define BORDER_SHOWING_AMT 4

void clampPlayerToWorld(Player *p, World *w){

}

void renderWorld(World *w, Camera *c, SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 7, 255, 4, 255);
    for(int i = 0; i< forest.propCount; i++){
        int propX = w->props[i].xPos - c->x;
        int propY = w->props[i].yPos - c->y;
        int propXMax = propX + w->props[i].body.w;
        int propYMax = propY + w->props[i].body.h;
        if( (propXMax > 0) && !(propX > c->window_width) && (propYMax > 0 ) && !(propY > c->window_height)){
            w->props[i].body.x = propX;
            w->props[i].body.y = propY;
            SDL_RenderFillRect(renderer, &w->props[i].body);
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    if(c->x <= w->collider[1].w) { SDL_RenderFillRect(renderer, &w->collider[1]); }
    if(c->y <= w->collider[0].w) { SDL_RenderFillRect(renderer, &w->collider[0]); }
    if(c->x >= w->width - (c->window_width)) { SDL_RenderFillRect(renderer, &w->collider[3]); }
    if(c->y >= w->height - (c->window_height)) { SDL_RenderFillRect(renderer, &w->collider[2]); }
}


World forest = { 
    .width=3000,
    .height=2000,
};

void generateForest(World *w, int count){

    int borderLineCount = 4;
    w->propCount = count;
    w->props = malloc(count * sizeof(Tree));
    if(w->props==NULL){
        fprintf(stderr, "ERROR: malloc function for the props did NOT work");
    }
    for(int i = 0; i < count; i++){
        int x = rand() % (w->width - TREE_W);
        int y = rand() % (w->height - TREE_H);
        w->props[i] = (Tree) { .xPos = x, .yPos = y, .body = { .w = TREE_W, .h = TREE_H } };
    }
    w->collider = malloc(borderLineCount * sizeof(SDL_Rect));
    
    w->collider[0] = (SDL_Rect) { .w = w->width, .h = 64, .x=-60, .y=-60 };
    w->collider[1] = (SDL_Rect) { .w = 64, .h = w->height, .x=-60, .y = -64 };
    w->collider[2] = (SDL_Rect) { .w = w->width, .h = 64, .x=-60, .y=w->height-12 };
    w->collider[3] = (SDL_Rect) { .w = 64, .h = w->height, .x=w->width-4, .y = -60 };

    w->colliderCount = borderLineCount;
}

void destoryForest(World *w){
    free(w->props);
    w->props = NULL;
}