#pragma once
#include <random>

#include "../Math/Vec3.hpp"

class Random
{
public :
    Random() = default;
    static Vec3 next(float min, float max);
};