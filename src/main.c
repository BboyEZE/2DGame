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
#include <stdint.h>

#include "config.h"

#include "player.h"
#include "projectile.h"
#include "vec2.h"
#include "camera.h"
#include "world.h"
#include "collision.h"
#include "render.h"

Uint32 lastTime = 0;

// Projectile list and count for number of projectiles
Projectile projectiles[MAX_SHOTS];

int projectileCount = 0;

float shotInterval = 0;
float shootingSpeed = 0;

//helper functions
void checkObsticleOverlap(Player *p, World *w, bool xDir, int prevLoc);
void keyDownEvent(const Uint8 *keyState, Player *p, float delta, World* w);

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
        tearDownAllRender(1);
        goto end;
    }

    status = initVideo();
    if(status) { goto end; }

    status = initGameWindow();
    if(status) { goto end; }
    status = initRenderer();
    if(status) { goto end; }
    status = makePlayerRender();
    if(status) { goto end; }

    initWindowScaling();

    srand(time(NULL));
    
    int running = 1;
    SDL_Event e;

    //make the player
    Player p1 = { .health = 100, .body = { forest.width/2, forest.height/2, PLAYER_SIZE_W, PLAYER_SIZE_H }, .pBodyImage = 0,.collisionBox = { forest.width/2, forest.height/2, PLAYER_SIZE_W, PLAYER_SIZE_H/4}};
    Camera cam = { .x=0, .y=0, .window_width = W_SCALE_WIDTH, .window_height = W_SCALE_HEIGHT };

    // make the forest world
    if(generateForest(&forest, 64) != 0){ goto end; }

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
        keyDownEvent(keyState, &p1, delta, world);

        //clamp the player position
        clampPlayerToWorld(&p1, world);

        //adjust the camera 
        updateCamera(&cam, &p1, world->width, world->height);
        //projectile movement
        int mouseX; int mouseY;
        float logicalMouseX; float logicalMouseY;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        setMouseVariableToMousePosition(&mouseX, &mouseY, &logicalMouseX, &logicalMouseY);

        if( (mouseState & SDL_BUTTON_LMASK) && projectileCount < MAX_SHOTS && canShootProjectile(delta)){
            int mouseWorldX = cam.x + mouseX;
            int mouseWorldY = cam.y + mouseY;
            Projectile newProjectile = fireProjectile(&p1, mouseWorldX, mouseWorldY);
            projectiles[projectileCount] = newProjectile;
            projectileCount++;    
        }

        int i = 0; 
        while(i<projectileCount){
            bool projectileAlive = moveProjectile(&projectiles[i], delta, world->width, world->height, PROJECTILE_SPEED) && !projectileIntersection(&projectiles[i].body, world);
            if(projectileAlive){
                i++;
                continue;
            }
            projectiles[i] = projectiles[projectileCount-1];
            projectileCount--;
        }

        clearRenderer();

        renderWorld(world, &p1, &cam);

        for(int i = 0; i < projectileCount; i++){
            renderProjectile(&projectiles[i]);
        }
        /*

        // render the player body
        SDL_SetRenderDrawColor(renderer, 155,155,0,255);
        setPlayerPhysicalPosition(&p1, &cam);
        SDL_RenderCopy(renderer, playerTexture, NULL, &p1.body);

        // render the screen

        */
        status = renderScreen();
        if(status != 0) { goto end; }
        
    }
    destoryForest(&forest);

    end: 
    return 1;
}

//void helpers

void keyDownEvent(const Uint8 *keyState, Player *p, float delta, World* w){
    int prevX = p->body.x;
    int prevY = p->body.y;
    if(keyState[SDL_SCANCODE_W]) { yDirectionMove(p, false, delta); }
    if(keyState[SDL_SCANCODE_S]) { yDirectionMove(p, true, delta); }

    checkObsticleOverlap(p, w, false, prevY);

    if(keyState[SDL_SCANCODE_A]) { xDirectionMove(p, false, delta); }
    if(keyState[SDL_SCANCODE_D]) { xDirectionMove(p, true, delta); }

    checkObsticleOverlap(p, w, true, prevX);
}

void checkObsticleOverlap(Player *p, World *w, bool xDir, int prevLoc){  
    Body *collider;
    bool collisionFound = false;
    if(w->props){
        for(int i = 0; i < w->propCount; i++){
            if(intersecting(&p->collisionBox, &w->props[i].collisionBox)) { 
                collider = &w->props[i].collisionBox; 
                collisionFound = true; 
            }
            
        }
    }

    if(collisionFound){
        if(!xDir){
            if(prevLoc + p->body.y < collider->y) { p->body.y = collider->y - p->body.h; }
            else { p->body.y = collider->y + collider->h; }
        } else {
            if(prevLoc + p->body.x < collider->x) { p->body.x = collider->x - p->body.w; }
            else { p->body.x = collider->x + collider->w; }
        }
    }

    
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