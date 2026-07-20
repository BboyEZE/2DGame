
#include <math.h>

#include "player.h"
#include "projectile.h"
#include "vec2.h"

Vec2 getProjectileDeltaDistance(float playerX, float playerY, int mouseX, int mouseY){
    Vec2 res = { mouseX - playerX, mouseY - playerY };
    return normalizeVecotor(res);
}

Vec2 normalizeVecotor(Vec2 original){
    float value = (original.x * original.x) + (original.y * original.y);
    float length = sqrtf(value);
    if(length == 0 ) { length = 1; };
    return (Vec2){ original.x / length, original.y / length}; 
}