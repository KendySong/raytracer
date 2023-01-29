#include "Graphics.hpp"
#include "Sphere.hpp"

Sphere::Sphere(Vec3 position, float radius, std::uint32_t color)
{
    this->color = color;
    this->position = position;
    this->radius = radius;
}

Sphere::Sphere(Vec3 position, float radius)
{
    this->color = Graphics::getColor(255, 0, 255);
    this->position = position;
    this->radius = radius;
}