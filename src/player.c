#include "player.h";

//adjust the position of the physical player body

void adjustPlayerBody(p){
    p->body.x = p->xPos;
    p->body.y = p->yPos;
}

void xDirectionMove(x, p){
     p->xPos += x;
     adjustPlayerBody(p);
}

void yDirectionMove(x, p){
    p->yPos += y;
    adjustPlayerBody(p);
}

void increaseHealth(h, p){
    p->health += h;
}

bool decreaseHealth(h, p){
    p->health -= h;
    if(p->health <= 0) { return false; }
    return true;
}