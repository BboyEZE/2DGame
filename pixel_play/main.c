#include <SDL2/SDL.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 720


#include <SDL2/SDL.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 720

typedef struct Tree {
    int xPos;
    int yPos;
} Tree;

void drawTree(Tree* t, SDL_Renderer *renderer){
    SDL_Rect leftBranch = (SDL_Rect) { t->xPos - 17, t->yPos - 39, 10, 15};
    SDL_Rect LargePart = (SDL_Rect) { t->xPos- 7, t->yPos - 44, 15, 17};
    SDL_Rect rightBranch = (SDL_Rect) { t->xPos+7, t->yPos - 39, 10, 15};
    SDL_Rect trunkFirst = (SDL_Rect) { t->xPos-7, t->yPos - 29, 15, 30};

    


    SDL_SetRenderDrawColor(renderer, 160, 82, 45, 255);
    SDL_RenderFillRect(renderer, &trunkFirst);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &leftBranch);
    SDL_RenderFillRect(renderer, &rightBranch);
    SDL_RenderFillRect(renderer, &LargePart);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect treeMarker = (SDL_Rect) { t->xPos, t->yPos, 1, 1 };
    SDL_RenderFillRect(renderer, &treeMarker);
}


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
    int running = 1;
    SDL_Event e;

    while (running) {
        // temporary settings for first initial game creation. Will be adjusted later on to be larger that the actual window.
        while(SDL_PollEvent(&e)){
            switch (e.type) {
                case SDL_QUIT:
                    running = 0; 
                    break;
            }
        }

        SDL_Rect leftBranch = (SDL_Rect) { 100, 100, 10, 15};
        SDL_Rect LargePart = (SDL_Rect) { 110, 95, 15, 17};
        SDL_Rect rightBranch = (SDL_Rect) { 125, 100, 10, 15};
        SDL_Rect trunkFirst = (SDL_Rect) { 110, 105, 15, 30};

        Tree t1 = { 120, 120 };
        Tree t2 = { 240, 240 };
        Tree t3 = { 532, 533 };

        SDL_RenderClear(renderer);
        
        drawTree(&t1, renderer);
        drawTree(&t2, renderer);
        drawTree(&t3, renderer);

        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderPresent(renderer);

    }
}