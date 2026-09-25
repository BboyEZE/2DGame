
#include "projectile.h"
#include "player.h"
#include "vec2.h"

void getDeltaMousePosition(Player* player, int mouseX, int mouseY, Projectile* p){
    Vec2 deltaPosition = getProjectileDeltaDistance(player->body.x, player->body.y, mouseX, mouseY);
    p->body.x = player->body.x;
    p->body.y = player->body.y;
    p->xAimDirection = deltaPosition.x;
    p->yAimDirection = deltaPosition.y;
}

bool moveProjectile(Projectile *p, float deltaTime, int widthMax, int heightMax, int speed){
    p->lifetime -= deltaTime;
    p->body.x += p->xAimDirection * deltaTime * speed;
    p->body.y += p->yAimDirection * deltaTime * speed;
    if(p->body.x <= 0 || p->body.x >= widthMax || p->body.y <= 0 || p->body.y >= heightMax || p->lifetime <= 0){ return false; }
    return true;
}


