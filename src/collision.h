#pragma once
#include <stdbool.h>
typedef struct Body {
    float x;
    float y;
    int w;
    int h;
} Body;

bool intersecting(Body *b1, Body *b2);