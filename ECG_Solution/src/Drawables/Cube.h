#pragma once

#define _USE_MATH_DEFINES
#include "Drawable.h"
#include <cmath>

class Cube: public Drawable {
public:
    Cube(float width, float height, float depth);
    void generateCube(float width, float height, float depth);
};