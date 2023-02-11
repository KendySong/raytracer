#pragma once
#include <cstdint>
#include "../Math/Vec3.hpp"
#include "Material.hpp"

struct Sphere
{
    Sphere(Vec3 position, float radius, Material material);
    Sphere(Vec3 position, float radius);

    Vec3 position;
    Material material;
    float radius;  
};