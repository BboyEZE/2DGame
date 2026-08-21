#pragma once
#include <stdbool.h>
typedef struct Thing {
    int left;
    int right;
    int top;
    int bot;
} Thing;

bool intersecting(Thing *obj1, Thing *obj2);