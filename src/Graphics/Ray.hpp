#pragma once
#include "../Math/Vec3.hpp"

struct Ray
{
public :
	Ray() = default;
	Ray(Vec3 origin, Vec3 direction);
	Vec3 at(float scalar);

	Vec3 origin;
	Vec3 direction;
};