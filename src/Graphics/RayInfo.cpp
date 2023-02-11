#include "RayInfo.hpp"

RayInfo::RayInfo(Sphere* sphere, float distance, Vec3 position, Vec3 normal)
{
	this->sphere = sphere;
	this->distance = distance;
	this->position = position;
	this->normal = normal;
}