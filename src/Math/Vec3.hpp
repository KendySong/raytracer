#pragma once
class Vec3
{
public :
    Vec3();
    Vec3(float all);
    Vec3(float x, float y, float z);

    void operator+=(const Vec3& vec) noexcept;
    void operator-=(const Vec3& vec) noexcept;
    void operator*=(const Vec3& vec) noexcept;
    void operator*=(float scalar) noexcept;
    void operator/=(const Vec3& vec) noexcept;

    float x;
    float y;
    float z;
};

Vec3 operator-(const Vec3& vec) noexcept;
Vec3 operator+(const Vec3& vec1, const Vec3& vec2) noexcept;
Vec3 operator-(const Vec3& vec1, const Vec3& vec2) noexcept;
Vec3 operator*(const Vec3& vec1, const Vec3& vec2) noexcept;
Vec3 operator*(const Vec3& vec, float scalar) noexcept;
Vec3 operator/(const Vec3& vec1, const Vec3& vec2) noexcept;
Vec3 operator/(const Vec3& vec, float scalar) noexcept;
bool operator==(Vec3& vec1, Vec3& vec2) noexcept;