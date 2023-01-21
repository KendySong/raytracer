#pragma once
class Vec2
{
public :
    Vec2();

    Vec2(float x, float y);  

    void operator+=(const Vec2& vec) noexcept;

    void operator-=(const Vec2& vec) noexcept;

    void operator*=(const Vec2& vec) noexcept;

    void operator/=(const Vec2& vec) noexcept;

    void operator/=(float scalar) noexcept;

    float x;
    float y;
};

Vec2 operator+(const Vec2& vec1, const Vec2& vec2);

Vec2 operator-(const Vec2& vec1, const Vec2& vec2);

Vec2 operator*(const Vec2& vec1, const Vec2& vec2);

Vec2 operator/(const Vec2& vec1, const Vec2& vec2);

Vec2 operator/(const Vec2& vec, float scalar);

Vec2 operator*(const Vec2& vec, float scalar);

bool operator==(Vec2& vec1, Vec2& vec2);