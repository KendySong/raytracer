#include "Ray.hpp"

Ray::Ray(Vec3 origin, Vec3 direction)
{
	this->origin = origin;
	this->direction = direction;
}

Vec3 Ray::at(float scalar) const
{
	return origin + direction * scalar;
}