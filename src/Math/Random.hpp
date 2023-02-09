#pragma once
#include <random>

#include "../Math/Vec3.hpp"

class Random
{
public :
    Random();
    Random(float min, float max);
    Vec3 next();

private :
    std::mt19937 m_randomEngine;
    std::uniform_real_distribution<float> m_distribution;
};