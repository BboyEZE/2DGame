#include "player.h"

void adjustPlayerBody(Player *p){
    p->body.x = p->xPos;
    p->body.y = p->yPos;
}

void xDirectionMove(Player *p, bool positive, float delta){
    int i = 1;
    if(!positive) { i = -1; }
     p->xPos += delta * i;
     adjustPlayerBody(p);
}

void yDirectionMove(Player *p, bool positive, float delta){
    int i = 1;
    if(!positive) { i = -1; }
    p->yPos += delta * i;
    adjustPlayerBody(p);
}

void increaseHealth(Player *p, int h){
    p->health += h;
}

bool decreaseHealth(Player *p, int h){
    p->health -= h;
    if(p->health <= 0) { return false; }
    return true;
}