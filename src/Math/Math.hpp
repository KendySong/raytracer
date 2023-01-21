#pragma once
#include "Vec2.hpp"
#include "Vec3.hpp"

class Math
{
public :
    //Vec2
    static float dot(const Vec2& vec1, const Vec2& vec2) noexcept;
    static float magnitude(const Vec2& vec) noexcept;
    static Vec2 normalize(const Vec2& vec) noexcept;

    //Vec3
    static Vec3 cross(const Vec3& vec1, const Vec3& vec2) noexcept;
    static float dot(const Vec3& vec1, const Vec3& vec2) noexcept;
    static float magnitude(const Vec3& vec) noexcept;
    static Vec3 normalize(const Vec3& vec) noexcept;

    static Vec3 rotateX(const Vec3& vec, float angle);
    static Vec3 rotateY(const Vec3& vec, float angle);
    static Vec3 rotateZ(const Vec3& vec, float angle);

    //Misc
    static float toRadian(float angle);
};