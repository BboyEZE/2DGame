// Five phases of main app development
// 1) Initialize SDL - turn on the video subsystem
// 2) Create a window - and a renderer attached to it (the thing that draws).
// 3) The game loop - keep running until the user quits.
// 4) Inside the loop: drain events, then draw a frame
// 5) Clean up - destory what is created, in reverse order, and shut SDL down
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>


#include "player.h"
#include "projectile.h"
#include "vec2.h"
#include "camera.h"
#include "world.h"
#include "collision.h"

Uint32 lastTime = 0;

/* PLAYER CONSTANTS */
#define PLAYER_SIZE_W 16
#define PLAYER_SIZE_H 24
// pixels per second
#define PLAYER_SPEED 150

/* PROJECTILE CONSTANTS */
#define PROJECTILE_SPEED 500
// possible limit to the number of bullets
#define MAX_SHOTS 20

// miliseconds until next shot
#define FIRE_INTERVAL 250
#define PROJECTILE_SHOOTING_SPEED 0.2
#define PROJECTILE_SIZE_W 6
#define PROJECTILE_SIZE_H 6

/* WINDOW CONSTANTS*/
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// scaling const
#define W_SCALE_WIDTH 640
#define W_SCALE_HEIGHT 360
// Projectile list and count for number of projectiles
Projectile projectiles[MAX_SHOTS];

int projectileCount = 0;

float shotInterval = 0;
float shootingSpeed = 0;

SDL_Renderer *renderer = NULL;
SDL_Window *gameWindow = NULL;
SDL_Surface *playerSurface = NULL;
SDL_Texture *playerTexture = NULL;


//helper functions
void keyDownEvent(const Uint8 *keyState, Player *p, float delta);
void setPlayerPhysicalPosition(Player *p, Camera* cam);

float getDelta();

bool canShootProjectile(float delta);

Projectile fireProjectile(Player *p, int mouseX, int mouseY);

int main(int argc, char *argv[]){
    int status = 0;
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "error: %s", SDL_GetError());  
        return 1;
    }
    if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
        fprintf(stderr, "error: failed to init image. (%s)\n",IMG_GetError());
        status = 1;
        goto cleanup_sdl;
    }
    gameWindow = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if(gameWindow == NULL){
        fprintf(stderr, "error: %s", SDL_GetError());
        status = 1;
        goto cleanup_img;
    }

    renderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL){

        fprintf(stderr, "error: %s", SDL_GetError());
        goto cleanup_window;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_RenderSetLogicalSize(renderer, W_SCALE_WIDTH, W_SCALE_HEIGHT);
    SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

    srand(time(NULL));
    
    int running = 1;
    SDL_Event e;

    playerSurface = IMG_Load("assets/img/characters/knight_character.png");
    if(playerSurface == NULL){
        fprintf(stderr, "error: error at IMG_LOAD - %s", IMG_GetError());
        goto cleanup_renderer;
    }
    SDL_Texture *playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
    SDL_FreeSurface(playerSurface);
    playerSurface = NULL;
    if(playerTexture == NULL){
        fprintf(stderr, "error: error at SDL create texture from surface - %s", SDL_GetError());
        goto cleanup_renderer;
    }

    //make the player
    Player p1 = { .health = 100, .body = { forest.width/2, forest.height/2, PLAYER_SIZE_W, PLAYER_SIZE_H }, .pBodyImage = playerTexture};
    Camera cam = { .x=0, .y=0, .window_width = W_SCALE_WIDTH, .window_height = W_SCALE_HEIGHT };

    // make the forest world
    if(generateForest(&forest, 64, renderer) != 0){ goto cleanup_texture; }

    // buffer to ensure print statements occur immediately.
    setvbuf(stdout, NULL, _IONBF, 0);

    shootingSpeed = PROJECTILE_SHOOTING_SPEED;

    while (running) {
        World *world = &forest;
        
        // temporary settings for first initial game creation. Will be adjusted later on to be larger that the actual window.

        while(SDL_PollEvent(&e)){
            switch (e.type) {
                case SDL_QUIT:
                    running = 0; 
                    break;
            }
        }
        //time passed between each frame. Used for all movements
        float delta = getDelta();

        // decrement shot interval so the system ALWAYS knows if the user can shoot.
        if(shotInterval > 0) { shotInterval -= delta; }

        //player movement
        const Uint8 *keyState = SDL_GetKeyboardState(NULL);
        keyDownEvent(keyState, &p1, delta);

        //clamp the player position
        clampPlayerToWorld(&p1, world);

        //adjust the camera 
        updateCamera(&cam, &p1, world->width, world->height);

        //projectile movement
        int mouseX; int mouseY;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        float logicalMouseX; float logicalMouseY;
        SDL_RenderWindowToLogical(renderer, mouseX, mouseY, &logicalMouseX, &logicalMouseY);
        mouseX = (int)(logicalMouseX);
        mouseY = (int)(logicalMouseY);

        if( (mouseState & SDL_BUTTON_LMASK) && projectileCount < MAX_SHOTS && canShootProjectile(delta)){
            int mouseWorldX = cam.x + mouseX;
            int mouseWorldY = cam.y + mouseY;
            Projectile newProjectile = fireProjectile(&p1, mouseWorldX, mouseWorldY);
            projectiles[projectileCount] = newProjectile;
            projectileCount++;    
        }

        int i = 0; 
        while(i<projectileCount){
            bool projectileAlive = moveProjectile(&projectiles[i], delta, world->width, world->height, PROJECTILE_SPEED) && !projectileIntersection(projectiles[i].xPos, projectiles[i].xPos + projectiles[i].body.w, projectiles[i].yPos, projectiles[i].yPos + projectiles[i].body.h, world);
            if(projectileAlive){
                i++;
                continue;
            }
            projectiles[i] = projectiles[projectileCount-1];
            projectileCount--;
        }

        SDL_RenderClear(renderer);

        renderWorld(world, &cam, renderer);

        SDL_SetRenderDrawColor(renderer, 128, 233, 91, 255);
        for(int i = 0; i < projectileCount; i++){
            projectiles[i].body.x = projectiles[i].body.x - cam.x;
            projectiles[i].body.y = projectiles[i].body.y - cam.y;
            SDL_RenderFillRect(renderer, &projectiles[i].body);
        }
        
        // render the player body
        SDL_SetRenderDrawColor(renderer, 155,155,0,255);
        setPlayerPhysicalPosition(&p1, &cam);
        SDL_RenderCopy(renderer, playerTexture, NULL, &p1.body);

        // render the screen
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderPresent(renderer);
    }
    destoryForest(&forest);

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

    return 1;
}

//void helpers

void keyDownEvent(const Uint8 *keyState, Player *p, float delta){
    int prevX = p->body.x;
    int prevY = p->body.y;
    if(keyState[SDL_SCANCODE_W]) { yDirectionMove(p, false, delta); }
    if(keyState[SDL_SCANCODE_A]) { xDirectionMove(p, false, delta); }
    if(keyState[SDL_SCANCODE_S]) { yDirectionMove(p, true, delta); }
    if(keyState[SDL_SCANCODE_D]) { xDirectionMove(p, true, delta); }


}

// creation of a new projectile for firing.
Projectile fireProjectile(Player* p, int mouseX, int mouseY){
    Vec2 projectileVector = getProjectileDeltaDistance(p->body.x, p->body.y, mouseX, mouseY);
    Projectile shot = {
        PROJECTILE_SPEED,
        .xAimDirection = projectileVector.x,
        .yAimDirection = projectileVector.y,
        .body = {p->body.x, p->body.y, PROJECTILE_SIZE_W, PROJECTILE_SIZE_H},
        .lifetime = 10,
    };
    return shot;
}

void setPlayerPhysicalPosition(Player *p, Camera *cam){
    p->body.x = p->body.x - cam->x;
    p->body.y = p->body.y - cam->y;
}

//float helpers

// get Delta time for multiple different functions
float getDelta(){
    Uint32 now = SDL_GetTicks();
    float delta = (now - lastTime) / 1000.0;
    lastTime = now;
    return delta;
    fprintf(stdout, "Current delta: %f ",delta);
}

bool canShootProjectile(float delta){
    if(shotInterval <= 0){
        shotInterval = shootingSpeed;
        return true;
    }
    else return false;
}