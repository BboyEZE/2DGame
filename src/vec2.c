

#include "player.h"
#include "projectile.h"
#include "vec2.h"

Vec2 getProjectileDeltaDistance(float playerX, float playerY, int mouseX, int mouseY){
    Vec2 res = { playerX - mouseX, playerY - mouseY };
    return res;
}