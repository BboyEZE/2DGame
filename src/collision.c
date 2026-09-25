#include "collision.h"

// intersection of two objects, one moving one not
bool intersecting(Body *b1, Body *b2){
    return (b1->x+b1->w > b2->x) && (b2->x + b2->w > b1->x) && (b1->y + b1->h > b2->y) && (b2->y + b2->h > b1->y);
}

bool xIntersection(Body *b1, Body *b2){
    return(b1->x+b1->w > b2->x) && (b2->x + b2->w > b1->x);
}

bool yIntersecting(Body *b1, Body *b2){
      return (b1->x+b2->w > b2->x) && (b2->x + b2->w > b1->x);
}

void mobMoving (Body *mob, Body *obj, float mPrevX, float mPrevY){
    
}

