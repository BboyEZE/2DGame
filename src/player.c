#include "player.h"
#include "world.h"
#define PLAYER_SPEED 250


void xDirectionMove(Player *p, bool positive, float delta){
    int i = 1;
    if(!positive) { i = -1; }
    p->body.x += delta * i * PLAYER_SPEED;
}

void yDirectionMove(Player *p, bool positive, float delta){
    int i = 1;
    if(!positive) { i = -1; }
    p->body.y += delta * i * PLAYER_SPEED;
}

void increaseHealth(Player *p, int h){
    p->health += h;
}

bool decreaseHealth(Player *p, int h){
    p->health -= h;
    if(p->health <= 0) { return false; }
    return true;
}
