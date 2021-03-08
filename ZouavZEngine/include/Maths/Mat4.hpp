#ifndef _MAT4_HPP_
#define _MAT4_HPP_

#include "Matrix.hpp"
#include <initializer_list>

class Vec3;
class Vec4;
class Quaternion;

class Mat4 : public Matrix
{
    public:
        Mat4();
        Mat4(std::initializer_list<float> matrix);
        Mat4(const Matrix& m);
        ~Mat4() = default;

        static const Mat4 identity;
        static const Mat4 zero;

        static Mat4 CreateScaleMatrix(const Vec3& v);
        static Mat4 CreateTranslationMatrix(const Vec3& v);
        static Mat4 CreateXRotationMatrix(float angle);
        static Mat4 CreateYRotationMatrix(float angle);
        static Mat4 CreateZRotationMatrix(float angle);
        static Mat4 CreateFixedAngleEulerRotationMatrix(const Vec3& v);
        static Mat4 CreateTRSMatrix(const Vec3& translation, const Vec3& rotation, const Vec3& scale);
        static Mat4 CreateTRSMatrix(const Vec3& translation, const Quaternion& rotate, const Vec3& scale);
        
        static Mat4 CreateUnitVecRotMatrix(const Vec3& v, float anle);
        static Mat4 CreateProjMatrix(float d);
        static Mat4 CreateOrthographicProjectionMatrix(float width, float height, float near, float far);
        static Mat4 CreatePerspectiveProjectionMatrix(float width, float height, float near, float far, float fov);

        Vec3 operator*(const Vec3& _v) const;
        Vec4 operator*(const Vec4& _v) const;
        Mat4 operator*(const Mat4& _m) const;
        Mat4 operator*(float _value) const;
        Mat4 operator/(float _value) const;

        void operator=(const Mat4& _m);
        void operator+=(const Mat4& _m);
        void operator-=(const Mat4& _m);
        void operator*=(const Mat4& _m);
        void operator*=(float _value);
        void operator/=(float _value);
};
#endif