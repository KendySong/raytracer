#pragma once
#include "../Math/Vec3.hpp"

struct Sphere
{
public :
    Sphere(Vec3 position, float radius, Vec3 color);
    Sphere(Vec3 position, float radius);

    Vec3 position;
    Vec3 color;
    float radius;  
};