#include "Maths/Vec4.hpp"

Vec4::Vec4()
    : w{ 1 }, x{ 0 }, y{ 0 }, z{ 0 }
{}

Vec4::Vec4(float _x, float _y, float _z)
    : w{ 1 }, x{ _x }, y{ _y }, z{ _z }
{}

Vec4::Vec4(float _w, float _x, float _y, float _z)
    : w{ _w }, x{ _x }, y{ _y }, z{ _z }
{}

Vec4::Vec4(const Vec3& v, float _w)
    : w{ _w }, x{ v.x }, y{ v.y }, z{ v.z }
{}

Vec4::Vec4(const Vec4& v)
    : w{ v.w }, x{ v.x }, y{ v.y }, z{ v.z }
{}