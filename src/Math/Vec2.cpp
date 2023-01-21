#include "Vec2.hpp"

Vec2::Vec2()
{
    x = 0;
    y = 0;
}

Vec2::Vec2(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Vec2::operator+=(const Vec2& vec) noexcept
{
    this->x += vec.x;
    this->y += vec.y;
}

void Vec2::operator-=(const Vec2& vec) noexcept
{
    this->x -= vec.x;
    this->y -= vec.y;
}

void Vec2::operator*=(const Vec2& vec) noexcept
{
    this->x *= vec.x;
    this->y *= vec.y;
}

void Vec2::operator/=(const Vec2& vec) noexcept
{
    this->x /= vec.x;
    this->y /= vec.y;
}

void Vec2::operator/=(float scalar) noexcept
{
    this->x /= scalar;
    this->y /= scalar;
}

Vec2 operator+(const Vec2& vec1, const Vec2& vec2)
{
    return {vec1.x + vec2.x, vec1.y + vec2.y};
}

Vec2 operator-(const Vec2& vec1, const Vec2& vec2)
{
    return {vec1.x - vec2.x, vec1.y - vec2.y};
}

Vec2 operator*(const Vec2& vec1, const Vec2& vec2)
{
    return {vec1.x * vec2.x, vec1.y * vec2.y};
}

Vec2 operator/(const Vec2& vec1, const Vec2& vec2)
{
    return {vec1.x / vec2.x, vec1.y / vec2.y};
}

Vec2 operator/(const Vec2& vec, float scalar)
{
    return { vec.x / scalar, vec.y / scalar };
}

Vec2 operator*(const Vec2& vec, float scalar)
{
    return { vec.x * scalar, vec.y * scalar };
}

bool operator==(Vec2& vec1, Vec2& vec2)
{
    return vec1.x == vec2.x && vec1.y == vec2.y;
}