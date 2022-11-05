#pragma once

#define _USE_MATH_DEFINES
#include "Drawable.h"
#include <cmath>

class Sphere: public Drawable {
public:
    Sphere(int longSegments, int latSegments, float radius);
    void generateSphere(int longSegments, int latSegments, float radius);
};

