#pragma once
#include "../Math/Vec3.hpp"

struct Material
{
    Material() = default;
    Material(Vec3 albedo, float roughness);

    Vec3 albedo;
    float roughness;
};