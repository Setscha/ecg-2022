#pragma once

#define _USE_MATH_DEFINES
#include "Drawable.h"
#include <cmath>

class Cylinder: public Drawable {
public:
    Cylinder(int segments, float height, float radius);
    void generateCylinder(int segments, float height, float radius);
};