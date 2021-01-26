#ifndef _VEC3_HPP_
#define _VEC3_HPP_

class Vec3
{
    public:
        float x;
        float y;
        float z;

        Vec3();
        Vec3(float _x, float _y, float _z);
        Vec3(const Vec3& v);
        ~Vec3() = default;

        float GetMagnitude() const;
        float GetSquaredMagnitude() const;
        Vec3 Normalized() const;
        Vec3 Cross(const Vec3& v) const;
        float Dot(const Vec3& v) const;
        float AngleToVector(const Vec3& v) const;  
        float DistanceToVector(const Vec3& v) const;
        bool Colinear(const Vec3& v) const;

        void Normalize();

        Vec3 operator+(const Vec3& v) const;
        Vec3 operator-(const Vec3& v) const;    
        Vec3 operator*(float value) const;

        void operator=(const Vec3& v);
        void operator+=(const Vec3& v);
        void operator-=(const Vec3& v);
        void operator*=(float value);
};

#endif