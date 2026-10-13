#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#include "config.h"

#include "render.h"
#include "projectile.h"
#include "player.h"
#include "world.h"

#include "collision.h"

SDL_Renderer *renderer = NULL;
SDL_Window *gameWindow = NULL;

SDL_Surface *playerSurface = NULL;
SDL_Surface *propSurface = NULL;

//texture constants
SDL_Texture *treeTexture = NULL;
SDL_Texture *playerTexture = NULL;

#define TILE_TEXTURE_COUNT 4
SDL_Texture *tileTextures[TILE_TEXTURE_COUNT];

//camera position
int camX;
int camY;

enum playerSkins { 
    KNIGHT,
    PIRATE,
    COWBOY
};

RenderObj visableObjs[128];
int visableObjsCount = 0;

void tearDownAllRender(char jump);

int initVideo(){
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ){
        fprintf(stderr, "video init error: %s", SDL_GetError());
        tearDownAllRender(2);
        return 1;
    }
    return 0;
} 

int initGameWindow(){ 
    gameWindow = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if(gameWindow == NULL){
        fprintf(stderr, "error: %s", SDL_GetError());
        tearDownAllRender(3);
        return 1;
    }
    return 0;
}

int initRenderer(){ 
    renderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL){
        fprintf(stderr, "error: %s", SDL_GetError());
        tearDownAllRender(4);
        return 1;
    }
    return 0;
}


void initWindowScaling (){
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_RenderSetLogicalSize(renderer, W_SCALE_WIDTH, W_SCALE_HEIGHT);
    SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

    SDL_SetRenderDrawColor(renderer, 7, 255, 4, 255);
}


void tearDownAllRender(char jump) {

    switch (jump){
        case 5: 
            goto cleanup_texture;
            break;
        case 4:
            goto cleanup_renderer;
            break;
        case 3:
            goto cleanup_window;
            break;
        case 2:
            goto cleanup_img;
            break;
        case 1:
            goto cleanup_sdl;
            break;
        default:
            break;
    }
    
    cleanup_texture:
    SDL_DestroyTexture(playerTexture);
    cleanup_renderer:
    SDL_DestroyRenderer(renderer);
    cleanup_window:
    SDL_DestroyWindow(gameWindow);
    cleanup_img:
    IMG_Quit();
    cleanup_sdl:
    SDL_Quit();

}

void setCameraPositionForRender(int x, int y){
    camX = x;
    camY = y;
}


//Texture creation for in game objects

int makePlayerRender(){
    playerSurface = IMG_Load("assets/img/characters/knight_character.png");
    if(playerSurface == NULL){
        fprintf(stderr, "error: error at IMG_LOAD - %s", IMG_GetError());
        return 1;
    }
    playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);
    playerSurface = NULL;
    if(playerTexture == NULL){
        fprintf(stderr, "error: error at SDL create texture from surface - %s", SDL_GetError());
        return 1;
    }
    return 0;
}

int createTileTextures(){
    //settings all differet ground texture rendering

    //dirt/grass ground
    propSurface = IMG_Load("assets/img/world/ground_test.png");
    if(propSurface == NULL){
        fprintf(stderr, "error: error at IMG_LOAD - %s", IMG_GetError());
        return 1;
    }
    tileTextures[0] = SDL_CreateTextureFromSurface(renderer, propSurface);
    propSurface = NULL;
    if(tileTextures[0] == NULL){
        fprintf(stderr, "error: error at SDL create texture from surface - %s", SDL_GetError());
        return 1;
    }

    //water ground
    propSurface = IMG_Load("assets/img/world/water_test.png");
    if(propSurface == NULL){
        fprintf(stderr, "error: error at IMG_LOAD - %s", IMG_GetError());
        return 1;
    }
    tileTextures[1] = SDL_CreateTextureFromSurface(renderer, propSurface);
    propSurface = NULL;
    if(tileTextures[1] == NULL){
        fprintf(stderr, "error: error at SDL create texture from surface - %s", SDL_GetError());
        return 1;
    }

    return 0;
}

int createForestTextures(){
    propSurface = IMG_Load("assets/img/world/tree_redone.png");
    if(propSurface == NULL){
        fprintf(stderr, "error: error at IMG_LOAD - %s", IMG_GetError());
        return 1;
    }
    treeTexture = SDL_CreateTextureFromSurface(renderer, propSurface);
    SDL_FreeSurface(propSurface);
    propSurface = NULL;
    if(treeTexture == NULL){
        fprintf(stderr, "error: error at SDL recate texture from surface - %s", SDL_GetError());
        return 1;
    }

    //settings all differet ground texture rendering
    if(createTileTextures() == 1) { return 1; }

    return 0;
}

void clearRenderer(){
    SDL_RenderClear(renderer);
}

// render calls for in game objects

void addObjToList(int kind, Body *body){
    int maxY = body->h + body->y;
    visableObjs[visableObjsCount] = (RenderObj) { .maxY = maxY, .kind = kind, .b = body};
    visableObjsCount++;
}

SDL_Rect createSDLRectBody(Body *b){
    return (SDL_Rect) { .x = floor(b->x) - camX, .y = floor(b->y) - camY, .w = b->w, .h = b->h };
}

void renderPlayer(Body *b){
    SDL_Rect playerBody = createSDLRectBody(b);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderCopy(renderer, playerTexture, NULL, &playerBody);
}

/* WORLD RENDERS */
void renderTree(Body *b){
    SDL_Rect treeBody = createSDLRectBody(b);
    SDL_RenderCopy(renderer, treeTexture, NULL, &treeBody);
}

void renderTile(unsigned char type, SDL_Rect *pos){
    SDL_RenderCopy(renderer, tileTextures[type], NULL, pos);
}

void renderBorder(Body *b){
    SDL_Rect borderBody = createSDLRectBody(b);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &borderBody);
}

void renderProjectile(Projectile *p){
    SDL_Rect projectileBody = createSDLRectBody(&p->body);
    SDL_SetRenderDrawColor(renderer, 128, 233, 91, 255);
    SDL_RenderDrawRect(renderer, &projectileBody);
    /* 
    ---- for once I create something for the projectile ----
    SDL_RenderCopy(renderer, projectileTexture, NULL, &projectileBody);
    */
    //
}

// destroy textures
void destoryTileTextures(){
    for(int i = 0; i < TILE_TEXTURE_COUNT; i++){
        SDL_DestroyTexture(tileTextures[i]);
    }
}

void destroyForestTextures(){
    SDL_DestroyTexture(treeTexture);
    destoryTileTextures();
}



void setMouseVariableToMousePosition(int *mouseX, int *mouseY, float *logicalMouseX, float *logicalMouseY){
        SDL_RenderWindowToLogical(renderer, *mouseX, *mouseY, logicalMouseX, logicalMouseY);
        *mouseX = (int)(*logicalMouseX);
        *mouseY = (int)(*logicalMouseY);
}

int comp(const void *a, const void *b){
     const RenderObj *objA = a;
     const RenderObj *objB = b;

     if(objA->maxY < objB->maxY) { return -1; }
     if(objA->maxY > objB->maxY) { return 1; }
     return 0;
}

int renderVisableObjs() {
    int status = 0;
    for(int i = 0; i < visableObjsCount; i++){
        switch(visableObjs[i].kind){
            case 0:
                renderPlayer(visableObjs[i].b);
                break;
            case 1:
                renderTree(visableObjs[i].b);
                break;
            default:
                status = 1;
                break;
        }
    }
    return status;
}

int renderScreen(){
    qsort(visableObjs, visableObjsCount, sizeof(visableObjs[0]), comp);
    //error out if something with an unexpected value is passed
    int status = renderVisableObjs();

    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderPresent(renderer);

    visableObjsCount = 0;

    return status;
}

