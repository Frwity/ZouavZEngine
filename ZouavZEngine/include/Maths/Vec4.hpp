#ifndef _VEC4_HPP_
#define _VEC4_HPP_

#include "Vec3.hpp"

class Vec4
{
    public:
        float w;
        float x;
        float y;
        float z;

        Vec4();
        Vec4(float _x, float _y, float _z);
        Vec4(float _w, float _x, float _y, float _z);
        Vec4(const Vec3& v, float _w);
        Vec4(const Vec4& v);
        ~Vec4() = default;
};
#endif