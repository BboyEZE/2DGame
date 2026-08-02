// Five phases of main app development
// 1) Initialize SDL - turn on the video subsystem
// 2) Create a window - and a renderer attached to it (the thing that draws).
// 3) The game loop - keep running until the user quits.
// 4) Inside the loop: drain events, then draw a frame
// 5) Clean up - destory what is created, in reverse order, and shut SDL down
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#include "player.h"
#include "projectile.h"
#include "vec2.h"
#include "camera.h"
#include "world.h"

Uint32 lastTime = 0;


/* PLAYER CONSTANTS */
#define PLAYER_SIZE_W 20
#define PLAYER_SIZE_H 30
// pixels per second
#define PLAYER_SPEED 300

/* PROJECTILE CONSTANTS */
#define PROJECTILE_SPEED 1000
// possible limit to the number of bullets
#define MAX_SHOTS 20

// miliseconds until next shot
#define FIRE_INTERVAL 250

#define PROJECTILE_SIZE_W 10
#define PROJECTILE_SIZE_H 10

/* WINDOW CONSTANTS*/
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// Projectile list and count for number of projectiles
Projectile projectiles[MAX_SHOTS];

int projectileCount = 0;

float shotInterval = 0;
float shootingSpeed = 0;


//helper functions
void keyDownEvent(const Uint8 *keyState, Player *p, float delta);
void setPlayerPhysicalPosition(Player *p, Camera* cam);

float getDelta();

bool canShootProjectile(float delta);

Projectile fireProjectile(Player *p, int mouseX, int mouseY);

int main(int argc, char *argv[]){
    int SDLInitSuccess = SDL_Init(SDL_INIT_VIDEO);
    if(SDLInitSuccess != 0){
        const char *errorMessage = SDL_GetError();
        fprintf(stderr, "error: %s", errorMessage);  
        return 1;  
    }
    SDL_Window *gameWindow = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if(gameWindow == NULL){
        const char *errorMessage = SDL_GetError();
        fprintf(stderr, "error: %s", errorMessage);
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL){
        const char *errorMessage = SDL_GetError();
        fprintf(stderr, "error: %s", errorMessage);
        return 1;
    }

    int levelWidth = 3000;
    int levelHeight = 2000;

    //make the player
    Player p1 = {.xPos = levelWidth/2,  .yPos= levelHeight/2, .health = 100, .body = { levelWidth/2, levelHeight/2, PLAYER_SIZE_W, PLAYER_SIZE_H }};
    Camera cam = { .x=0, .y=0, .window_width = WINDOW_WIDTH, .window_height = WINDOW_HEIGHT };
      

    int running = 1;
    SDL_Event e;

    // buffer to ensure print statements occur immediately.
    setvbuf(stdout, NULL, _IONBF, 0);

    shootingSpeed = .2;

    while (running) {
        
        
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

        //adjust the camera 
        updateCamera(&cam, &p1, levelWidth, levelHeight);

        //projectile movement
        int mouseX; int mouseY;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

        if( (mouseState & SDL_BUTTON_LMASK) && projectileCount < MAX_SHOTS && canShootProjectile(delta)){
            int mouseWorldX = cam.x + mouseX;
            int mouseWorldY = cam.y + mouseY;
            Projectile newProjectile = fireProjectile(&p1, mouseWorldX, mouseWorldY);
            projectiles[projectileCount] = newProjectile;
            projectileCount++;    
        }

        int i = 0;
        while(i<projectileCount){
            bool projectileAlive = moveProjectile(&projectiles[i], delta, levelWidth, levelHeight, PROJECTILE_SPEED);
            if(projectileAlive){
                i++;
                continue;
            }
            projectiles[i] = projectiles[projectileCount-1];
            projectileCount--;
        }

        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 128, 233, 91, 255);
        for(int i = 0; i < projectileCount; i++){
            projectiles[i].body.x = projectiles[i].xPos - cam.x;
            projectiles[i].body.y = projectiles[i].yPos - cam.y;
            SDL_RenderFillRect(renderer, &projectiles[i].body);
        }
        
        // render the player body
        SDL_SetRenderDrawColor(renderer, 155,155,0,255);
        setPlayerPhysicalPosition(&p1, &cam);
        SDL_RenderFillRect(renderer, &p1.body);

        // render the screen
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderPresent(renderer);
    }
    return 1;
}

//void helpers

void keyDownEvent(const Uint8 *keyState, Player *p, float delta){
    if(keyState[SDL_SCANCODE_W]) { yDirectionMove(p, false, delta); }
    if(keyState[SDL_SCANCODE_A]) { xDirectionMove(p, false, delta); }
    if(keyState[SDL_SCANCODE_S]) { yDirectionMove(p, true, delta); }
    if(keyState[SDL_SCANCODE_D]) { xDirectionMove(p, true, delta); }

}

// creation of a new projectile for firing.
Projectile fireProjectile(Player* p, int mouseX, int mouseY){
    Vec2 projectileVector = getProjectileDeltaDistance(p->xPos, p->yPos, mouseX, mouseY);
    Projectile shot = {
        p->xPos,
        p->yPos,
        PROJECTILE_SPEED,
        .xAimDirection = projectileVector.x,
        .yAimDirection = projectileVector.y,
        .body = {0, 0, PROJECTILE_SIZE_W, PROJECTILE_SIZE_H},
        .lifetime = 10,
    };
    return shot;
}

void setPlayerPhysicalPosition(Player *p, Camera *cam){
    p->body.x = p->xPos - cam->x;
    p->body.y = p->yPos - cam->y;
}

//float helpers

// get Delta time for multiple different functions
float getDelta(){
    Uint32 now = SDL_GetTicks();
    float delta = (now - lastTime) / 1000.0;
    lastTime = now;
    return delta;
    fprintf(stdout, "Current delta: ",delta);
}

bool canShootProjectile(float delta){
    if(shotInterval <= 0){
        shotInterval = shootingSpeed;
        return true;
    }
    else return false;
}