
#include "projectile.h"
#include "player.h"
#include "vec2.h"

void getDeltaMousePosition(Player* player, int mouseX, int mouseY, Projectile* p){
    Vec2 deltaPosition = getProjectileDeltaDistance(player->xPos, player->yPos, mouseX, mouseY);
    p->xPos = player->xPos;
    p->yPos = player->yPos;
    p->xAimDirection = deltaPosition.x;
    p->yAimDirection = deltaPosition.y;
}

bool moveProjectile(Projectile *p, float deltaTime, int widthMax, int heightMax, int speed){
    p->lifetime -= deltaTime;
    p-> xPos += p->xAimDirection * deltaTime * speed;
    p-> yPos += p->yAimDirection * deltaTime * speed;
    if(p->xPos <= 0 || p->xPos >= widthMax || p->yPos <= 0 || p->yPos >= heightMax || p->lifetime <= 0){ return false; }
    return true;
}


