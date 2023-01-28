#pragma once
#include <cstdint>
#include "../Math/Vec3.hpp"

struct Sphere
{
public :
    Sphere(Vec3 position, float radius, std::uint32_t color);
    Sphere(Vec3 position, float radius);

    Vec3 position;
    std::uint32_t color;
    float radius;  
};