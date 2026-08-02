#include "player.h"
#include "vec2.h"
#include "camera.h"

void updateCamera(Camera* camera, Player* player, const int worldW, const int worldH){
    camera->x = player->xPos - camera->window_width / 2;
    camera->y = player->yPos - camera->window_height / 2;
    cameraAdjustWorld(camera, worldW, worldH);
}

void cameraAdjustWorld(Camera* camera, const int worldW, const int worldH){
    int maxX = worldW - camera->window_width;
    int maxY = worldH - camera->window_height;

    if(camera->x < 0) { camera->x = 0; }
    else if(camera->x > maxX) { camera->x = maxX; }

    if(camera->y < 0) { camera->y = 0; }
    else if(camera->y > maxY) { camera->y = maxY; }
}

