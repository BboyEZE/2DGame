//Information for the player and all the things you can do to the player
//definition for the player

#include <SDL2/SDL.h>

const int PLAYER_SIZE_W = 20;
const int PLAYER_SIZE_H = 30;

const int PLAYER_SPEED = 5;

typedef struct Player {
        int xPos;
        int yPos;
        int health;
        SDL_Rect body;
} Player;

void xDirectionMove(int x, &Player p);
void yDirectionMove(int y, &Player p);
void xyDirectionMove(int x, int y, &Player p);
bool healthDecrease(int h, &Player p);
void healthIncrease(int h, &Player p);
void setPosition(int x, int y, &Player p);
