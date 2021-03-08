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
        Vec3(const Vec3& _v);
        ~Vec3() = default;

        const static Vec3 zero;
        const static Vec3 one;
        const static Vec3 forward;
        const static Vec3 backward;
        const static Vec3 right;
        const static Vec3 left;
        const static Vec3 up;
        const static Vec3 down;

        float GetMagnitude() const;
        float GetSquaredMagnitude() const;
        Vec3 Normalized() const;
        Vec3 Cross(const Vec3& _v) const;
        float Dot(const Vec3& _v) const;
        float AngleToVector(const Vec3& _v) const;  
        float DistanceToVector(const Vec3& _v) const;
        bool Colinear(const Vec3& _v) const;

        void Normalize();

        Vec3 operator+(const Vec3& v) const;
        Vec3 operator-(const Vec3& v) const;    
        Vec3 operator*(const Vec3& v) const;
        Vec3 operator*(float _value) const;
        Vec3 operator/(float _value) const;

        void operator=(const Vec3& _v);
        void operator+=(const Vec3& _v);
        void operator-=(const Vec3& _v);
        void operator*=(const Vec3& _v);
        void operator*=(float _value);
        void operator/=(float _value);

        Vec3 operator-() const;
};

Vec3 operator*(float _number, const Vec3& _rhs);

#endif