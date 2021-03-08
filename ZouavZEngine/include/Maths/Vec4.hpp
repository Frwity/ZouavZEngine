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

        static const Vec4 zero;
        static const Vec4 one;

        Vec4();
        Vec4(float _x, float _y, float _z);
        Vec4(float _w, float _x, float _y, float _z);
        Vec4(const Vec3& v, float _w);
        Vec4(const Vec4& v);
        ~Vec4() = default;

        Vec4 Normalized() const;
        void Normalize();
        float GetMagnitude() const;
        float GetSquaredMagnitude() const;

        Vec4 operator+(const Vec4& v) const;
        Vec4 operator-(const Vec4& v) const;    
        Vec4 operator*(const Vec4& v) const;
        Vec4 operator*(float _value) const;
        Vec4 operator/(float _value) const;

        Vec4 operator-() const;

        void operator=(const Vec4& _v);
        void operator+=(const Vec4& _v);
        void operator-=(const Vec4& _v);
        void operator*=(const Vec4& _v);
        void operator*=(float _value);
        void operator/=(float _value);
};
#endif