#pragma once
#include "Sphere.hpp"
#include "../Math/Vec3.hpp"

struct RayInfo
{
    RayInfo() = default;
    RayInfo(Sphere* sphere, float distance, Vec3 position, Vec3 normal);

    float distance; 
    Vec3 position;
    Sphere* sphere;
    Vec3 normal;
};