#ifndef _MATRIX4_HPP_
#define _MATRIX4_HPP_

#include "Matrix.hpp"

class Matrix4 : public Matrix
{
    public:
        Matrix4();
        Matrix4(const Matrix& m);
        ~Matrix4() = default;

        static Matrix4 CreateScaleMatrix(const Vec3& v);
        static Matrix4 CreateTranslationMatrix(const Vec3& v);
        static Matrix4 CreateXRotationMatrix(float angle);
        static Matrix4 CreateYRotationMatrix(float angle);
        static Matrix4 CreateZRotationMatrix(float angle);
        static Matrix4 CreateFixedAngleEulerRotationMatrix(const Vec3& v);
        static Matrix4 CreateTRSMatrix(const Vec3& translation, const Vec3& rotation, const Vec3& scale);
        static Matrix4 CreateUnitVecRotMatrix(const Vec3& v, float anle);
        static Matrix4 CreateProjMatrix(float d);
        static Matrix4 CreateOrthographicProjectionMatrix(int width, int height, float near, float far);
        static Matrix4 CreatePerspectiveProjectionMatrix(int width, int height, float near, float far, float fov);
        static Matrix4 Identity();
        static Matrix4 Zero();

        Vec3    operator*(const Vec3& v) const;
        Matrix4 operator*(const Matrix4& m) const;
        void    operator=(const Matrix4& m);
};
#endif