#include "Random.hpp"

//https://stackoverflow.com/questions/686353/random-float-number-generation
Vec3 Random::next(float min, float max)
{
    return
    {
        min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min))),
        min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min))),
        min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)))
    };
}