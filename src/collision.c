
#include "collision.h"

bool intersecting(Thing *obj1, Thing *obj2){
    return (obj1->right > obj2->left) && (obj2->right > obj1->left) && (obj1->bot > obj2->top) && (obj2->bot > obj1->top);
}

