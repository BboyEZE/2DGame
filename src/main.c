// Five phases of main app development
// 1) Initialize SDL - turn on the video subsystem
// 2) Create a window - and a renderer attached to it (the thing that draws).
// 3) The game loop - keep running until the user quits.
// 4) Inside the loop: drain events, then draw a frame
// 5) Clean up - destory what is created, in reverse order, and shut SDL down
#include <SDL2/SDL.h>
#include <stdio.h>

void playerMoveRight(Player* p){ p->positionX = p->positionX + PLAYER_SPEED; }
void playerMoveLeft(Player* p){ p->positionX = p->positionX - PLAYER_SPEED; }
void playerMoveUp (Player* p) { p->positionY = p->positionY + PLAYER_SPEED; }
void playerMoveDown(Player* p){ p->positionY = p->positionY - PLAYER_SPEED; }
void setPlayerPosition(Player* p, int x, int y) {
    p->xPos = x;
    p->yPos = y;
}


int main(int argc, char *argv[]){
    int SDLInitSuccess = SDL_Init(SDL_INIT_VIDEO);
    if(SDLInitSuccess != 0){
        const char *errorMessage = SDL_GetError();
        fprintf(stderr, "error: %s", errorMessage);  
        return 1;  
    }
    SDL_Window *gameWindow = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    if(gameWindow == NULL){
        const char *errorMessage = SDL_GetError();
        fprintf(stderr, "error: %s", errorMessage);
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL){
        const char *errorMessage = SDL_GetError();
        fprintf(stderr, "error: %s", errorMessage);
        return 1;
    }

    int running = 1;
    SDL_Event e;

    Player p1 = {.xPos = 300,  .yPost= 400, .health = 100, .body = {300, 400, 15, 19}};


    while (running) {
        
        
        while(SDL_PollEvent(&e)){
            switch (e.type) {
                case SDL_QUIT:
                    running = 0; 
                    break;
                case SDL_KEYDOWN:
                    printf("Key Pressed \n");
                    break;
                case SDL_KEYUP:
                    printf("Key Released \n");
                    break;
                case SDL_MOUSEMOTION:
                    printf("Mouse Moved \n");
                    break;
                case SDL_MOUSEBUTTONDOWN: 
                    printf("Mouse button clicked \n"); 
                    break;
                case SDL_MOUSEBUTTONUP:  
                    printf("Mouse button released \n"); 
                    break;
                
            }

        }
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 155,155,0,255);
        
        setPlayerPhysicalPosition(&player);
        SDL_RenderFillRect(renderer, player.body);

        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderPresent(renderer);
    }
}

