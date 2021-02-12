#ifndef _MAT4_HPP_
#define _MAT4_HPP_

#include "Matrix.hpp"
#include <initializer_list>

class Vec3;
class Quaternion;

class Mat4 : public Matrix
{
    public:
        Mat4();
        Mat4(std::initializer_list<float> matrix);
        Mat4(const Matrix& m);
        ~Mat4() = default;

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
        static Mat4 Identity();
        static Mat4 Zero();

        Vec3 operator*(const Vec3& v) const;
        Mat4 operator*(const Mat4& m) const;
        Mat4 operator/(float) const;
        void operator=(const Mat4& m);
};
#endif