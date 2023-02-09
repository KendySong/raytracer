#include "Graphics.hpp"
#include "Sphere.hpp"

Sphere::Sphere(Vec3 position, float radius, Material material)
{
    this->material = material;
    this->position = position;
    this->radius = radius;
}

Sphere::Sphere(Vec3 position, float radius)
{
    this->material = Material(Vec3(255, 0, 255), 0, 0);
    this->position = position;
    this->radius = radius;
}