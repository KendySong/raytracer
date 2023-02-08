#define _USE_MATH_DEFINES
#include <cmath>

#include "Math.hpp"

float Math::dot(const Vec2& vec1, const Vec2& vec2) noexcept
{
    return (vec1.x * vec2.y) + (vec1.y * vec2.y);
}

float Math::magnitude(const Vec2& vec) noexcept
{
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

Vec2 Math::normalize(const Vec2& vec) noexcept
{
    float length = Math::magnitude(vec);
    return vec / length;
}

Vec3 Math::cross(const Vec3& vec1, const Vec3& vec2) noexcept
{
    return 
    {
        vec1.y * vec2.z - vec1.z * vec2.y, 
        vec1.z * vec2.x - vec1.x * vec2.z, 
        vec1.x * vec2.y - vec1.y * vec2.x 
    };
}

float Math::dot(const Vec3& vec1, const Vec3& vec2) noexcept
{
    return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
}

float Math::length(const Vec3& vec) noexcept
{
    return sqrt(vec.x * vec.x + vec.z * vec.z + vec.y * vec.y);
}

Vec3 Math::normalize(const Vec3& vec) noexcept
{
    float length = Math::length(vec);
    return vec / length;
}

Vec3 Math::rotateX(const Vec3& vec, float angle)
{
    angle += atan2f(vec.y, vec.z);
    float length = Math::magnitude({ vec.y, vec.z });

    Vec3 rotated
    {
        vec.x,
        length * sin(angle),
        length * cos(angle)
    };
    return rotated;
}

Vec3 Math::rotateY(const Vec3& vec, float angle)
{
    angle += atan2(vec.z, vec.x);
    float length = Math::magnitude({ vec.x, vec.z });

    Vec3 rotated
    {
        length * sin(angle),
        vec.y,
        length * cos(angle)
    };
    return rotated;
}

Vec3 Math::rotateZ(const Vec3& vec, float angle)
{
    angle += atan2f(vec.y, vec.x);
    float length = Math::magnitude({ vec.x, vec.y });

    Vec3 rotated
    {
        length * sin(angle),
        length * cos(angle),
        vec.z
    };
    return rotated;  
}

float Math::toRadian(float angle)
{
    return angle * (M_PI / 180);
}

Vec3 Math::reflect(const RayInfo& rayInfo)
{
    Vec3 normal = Math::normalize(rayInfo.position - rayInfo.sphere->position);
    return rayInfo.position - 2 * Math::dot(rayInfo.position, normal) * normal;
}