#include "Vec3.hpp"

Vec3::Vec3()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vec3::Vec3(float all)
{
    this->x = all;
    this->y = all;
    this->z = all;
}

Vec3::Vec3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vec3::operator+=(const Vec3& vec) noexcept
{
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
}

void Vec3::operator-=(const Vec3& vec) noexcept
{
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
}

void Vec3::operator*=(const Vec3& vec) noexcept
{
    this->x *= vec.x;
    this->y *= vec.y;
    this->z *= vec.z;
}

void Vec3::operator*=(float scalar) noexcept
{
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
}

void Vec3::operator/=(const Vec3& vec) noexcept
{
    this->x /= vec.x;
    this->y /= vec.y;
    this->z /= vec.z;
}

Vec3 operator-(const Vec3& vec) noexcept
{
    return {-vec.x, -vec.y, -vec.z};
}

Vec3 operator+(const Vec3& vec1, const Vec3& vec2) noexcept
{
    return {vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z};
}

Vec3 operator-(const Vec3& vec1, const Vec3& vec2) noexcept
{
    return {vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z};
}

Vec3 operator*(const Vec3& vec1, const Vec3& vec2) noexcept
{
    return {vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z};
}

Vec3 operator*(const Vec3& vec, float scalar) noexcept
{
    return { vec.x * scalar , vec.y * scalar , vec.z * scalar };
}

Vec3 operator/(const Vec3& vec1, const Vec3& vec2) noexcept
{
    return {vec1.x / vec2.x, vec1.y / vec2.y, vec1.z / vec2.z};
}

Vec3 operator/(const Vec3& vec, float scalar) noexcept
{
    return { vec.x / scalar, vec.y / scalar, vec.z / scalar };
}

bool operator==(Vec3& vec1, Vec3& vec2) noexcept
{
    return vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z;
}