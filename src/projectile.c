
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

bool moveProjectilePosition(Projectile *p, float deltaTime){
    p-> xPos = p->xAimDirection * deltaTime;
    p-> yPos = p->yAimDirection * deltaTime;
}

bool moveProjectileBody(Projectile *p){
    if(p->xPos <= 0 || p->xPos >= {widthMax} || p->yPos <= 0 || p->yPos >= {height Max}){ return false; }
    //still within the screen bounds
    p->body.x = p->xPos;
    p->body.y = p->yPos;
    return true;
}

