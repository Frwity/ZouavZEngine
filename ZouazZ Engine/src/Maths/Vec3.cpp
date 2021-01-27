#include "Maths/Vec3.hpp"
#include <cmath>

Vec3::Vec3()
    : x{ 0 }, y{ 0 }, z{ 0 }
{}

Vec3::Vec3(float _x, float _y, float _z)
    : x{ _x }, y{ _y }, z{ _z }
{}

Vec3::Vec3(const Vec3& v)
    : x{ v.x }, y{ v.y }, z{ v.z }
{}

Vec3 Vec3::Zero()
{
    return { 0.0f, 0.0f, 0.0f };
}

Vec3 Vec3::One()
{
    return { 1.0f, 1.0f, 1.0f };
}

Vec3 Vec3::Forward()
{
    return { 0.0f, 0.0f, 1.0f };
}

Vec3 Vec3::Backward()
{
    return { 0.0f, 0.0f, -1.0f };
}

Vec3 Vec3::Right()
{
    return { 1.0f, 0.0f, 0.0f };
}

Vec3 Vec3::Left()
{
    return { -1.0f, 0.0f, 0.0f };
}

Vec3 Vec3::Up()
{
    return { 0.0f, 1.0f, 0.0f };
}

Vec3 Vec3::Down()
{
    return { 0.0f, -1.0f, 0.0f };
}

float Vec3::GetMagnitude() const
{
    return sqrt( x * x + y * y + z * z );
}

float Vec3::GetSquaredMagnitude() const
{
    return x * x + y * y + z * z;
}

void Vec3::Normalize()
{
    float length = GetMagnitude();
    x /= length;
    y /= length;
    z /= length;
}

Vec3 Vec3::Normalized() const
{
    float length = GetMagnitude();
    return { x / length, y / length, z / length };
}

Vec3 Vec3::operator+(const Vec3& v) const
{
    return { x + v.x, y  + v.y, z + v.z };
}

Vec3 Vec3::operator-(const Vec3& v) const
{
    return { x - v.x, y - v.y, z - v.z };
}

Vec3 Vec3::operator*(float value) const
{
    return { x * value, y * value, z * value };
}

void Vec3::operator=(const Vec3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
} 

void Vec3::operator+=(const Vec3& v)
{
    *this = *this + v;
}

void Vec3::operator-=(const Vec3& v)
{
    *this = *this - v;
}

void Vec3::operator*=(float value)
{
    *this = *this * value;
}

Vec3 Vec3::Cross(const Vec3& v) const
{
    return { y * v.z - z * v.y,
             z * v.x - x * v.z,
             x * v.y - y * v.x };
} 

float Vec3::Dot(const Vec3& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

float Vec3::AngleToVector(const Vec3& v) const
{
    return acos( Dot(v) / ( GetMagnitude() * v.GetMagnitude() ) );
}

float Vec3::DistanceToVector(const Vec3& v) const
{
    return sqrt( ( ( v.x - x ) * ( v.x - x ) ) + ( ( v.y - y ) * ( v.y - y ) ) + ( ( v.z - z ) * ( v.z - z ) ) );
}

bool Vec3::Colinear(const Vec3& vector) const
{
    float colinearFactor = Cross(vector).GetMagnitude();

    return (colinearFactor > -0.1 && colinearFactor < 0.1);
}