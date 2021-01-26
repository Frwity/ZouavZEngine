#include "Maths/Vec2.hpp"
#include <cmath>

Vec2::Vec2()
    : x{ 0 }, y{ 0 }
{}

Vec2::Vec2(float _x, float _y)
    : x{ _x }, y{ _y }
{}

Vec2::Vec2(const Vec2& v)
    : x{ v.x }, y{ v.y }
{}

float Vec2::GetMagnitude() const
{
    return sqrt( x * x + y * y );
}

float Vec2::GetSquaredMagnitude() const
{
    return x * x + y * y;
}

void Vec2::Normalize()
{
    float length = GetMagnitude();
    x /= length;
    y /= length;
}

Vec2 Vec2::Normalized() const
{
    float length = GetMagnitude();
    return { x / length, y / length };
}

float Vec2::Cross(const Vec2& v) const
{
    return x * v.y - v.x * y;
}

Vec2 Vec2::operator+(const Vec2& v) const
{
    return { x + v.x, y + v.y };
}

Vec2 Vec2::operator-(const Vec2& v) const
{
    return { x - v.x, y - v.y };
}

Vec2 Vec2::operator*(float value) const
{
    return { x * value, y * value };
}

void Vec2::operator=(const Vec2& v)
{
    x = v.x;
    y = v.y;
}

void Vec2::operator+=(const Vec2& v)
{
    *this = *this + v;
}

void Vec2::operator-=(const Vec2& v)
{
    *this = *this - v;
}

void Vec2::operator*=(float value)
{
    *this = *this * value;
}