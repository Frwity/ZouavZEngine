#ifndef _VEC2_HPP_
#define _VEC2_HPP_

#include <string>

class Vec2
{
    public:
        float x;
        float y;

        Vec2();
        Vec2(float _x, float _y);
        Vec2(const Vec2& v);
        ~Vec2() = default;

        float GetMagnitude() const;
        float GetSquaredMagnitude() const;
        Vec2 Normalized() const;
        float Cross(const Vec2&) const;

        void Normalize();

        std::string ToString() const;

        Vec2 operator+(const Vec2& v) const;
        Vec2 operator-(const Vec2& v) const;    
        Vec2 operator*(float value) const;

        void operator=(const Vec2& v);
        void operator+=(const Vec2& v);
        void operator-=(const Vec2& v);
        void operator*=(float value);
};

#endif