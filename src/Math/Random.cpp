#include "Random.hpp"

Random::Random()
{
    m_randomEngine.seed(std::random_device()());
}

Random::Random(float min, float max)
{
    m_randomEngine.seed(std::random_device()());
    m_distribution = std::uniform_real_distribution<float>(min, max);
}

Vec3 Random::next()
{
    return {
        m_distribution(m_randomEngine),
        m_distribution(m_randomEngine),
        m_distribution(m_randomEngine)
    };
}