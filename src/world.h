
#pragma once
#include <SDL2/SDL.h>

#define PROP_COUNT 64

typedef struct World {
    int width;
    int height;
    SDL_Rect props[PROP_COUNT];
    int proopCount = PROP_COUNT;
} World;
