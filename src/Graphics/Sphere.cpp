#include "Sphere.hpp"

Sphere::Sphere(Vec3 position, float radius, Vec3 color)
{
    this->color = color;
    this->position = position;
    this->radius = radius;
}

Sphere::Sphere(Vec3 position, float radius)
{
    this->color = Vec3(0, 255, 0);
    this->position = position;
    this->radius = radius;
}