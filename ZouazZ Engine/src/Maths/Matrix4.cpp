#include "Maths/Matrix4.hpp"
#include <math.h>

Matrix4::Matrix4() 
    : Matrix(4, 4)
{
    for (int i = 0; i < 16; i++)
        matrix[i] = 0;        
}

Matrix4::Matrix4(const Matrix& m) 
    : Matrix(m)
{}

Matrix4 Matrix4::CreateScaleMatrix(const Vec3& scale)
{
    Matrix4 scaleMat;

    scaleMat.Accessor(0, 0) = scale.x;
    scaleMat.Accessor(1, 1) = scale.y;
    scaleMat.Accessor(2, 2) = scale.z;
    scaleMat.Accessor(3, 3) = 1;

    return scaleMat;
}

Matrix4 Matrix4::CreateTranslationMatrix(const Vec3& translation)
{
    Matrix4 translationMat;

    translationMat.Accessor(0, 0) = 1;
    translationMat.Accessor(1, 1) = 1;
    translationMat.Accessor(2, 2) = 1;
    translationMat.Accessor(3, 3) = 1;

    translationMat.Accessor(0, 3) = translation.x;
    translationMat.Accessor(1, 3) = translation.y;
    translationMat.Accessor(2, 3) = translation.z;

    return translationMat;
}

Matrix4 Matrix4::CreateXRotationMatrix(float angle)
{
    Matrix4 rotationMat;

    rotationMat.Accessor(0, 0) = 1;
    rotationMat.Accessor(3, 3) = 1;

    rotationMat.Accessor(1, 1) = cos(angle);
    rotationMat.Accessor(1, 2) = -sin(angle);
    rotationMat.Accessor(2, 1) = sin(angle);
    rotationMat.Accessor(2, 2) = cos(angle);

    return rotationMat;    
}

Matrix4 Matrix4::CreateYRotationMatrix(float angle)
{
    Matrix4 rotationMat;

    rotationMat.Accessor(1, 1) = 1;
    rotationMat.Accessor(3, 3) = 1;

    rotationMat.Accessor(0, 0) = cos(angle);
    rotationMat.Accessor(0, 2) = sin(angle);
    rotationMat.Accessor(2, 0) = -sin(angle);
    rotationMat.Accessor(2, 2) = cos(angle);

    return rotationMat;    
}

Matrix4 Matrix4::CreateZRotationMatrix(float angle)
{
    Matrix4 rotationMat;

    rotationMat.Accessor(2, 2) = 1;
    rotationMat.Accessor(3, 3) = 1;

    rotationMat.Accessor(0, 0) = cos(angle);
    rotationMat.Accessor(0, 1) = -sin(angle);
    rotationMat.Accessor(1, 0) = sin(angle);
    rotationMat.Accessor(1, 1) = cos(angle);

    return rotationMat;    
}

Matrix4 Matrix4::CreateFixedAngleEulerRotationMatrix(const Vec3& angle)
{
    return Matrix4::CreateYRotationMatrix(angle.y) * 
        Matrix4::CreateXRotationMatrix(angle.x) * 
        Matrix4::CreateZRotationMatrix(angle.z);    
}

Matrix4 Matrix4::CreateTRSMatrix(const Vec3& translation, const Vec3& rotate, const Vec3& scale)
{
    return Matrix4::CreateTranslationMatrix(translation) * 
        Matrix4::CreateFixedAngleEulerRotationMatrix(rotate) * 
        Matrix4::CreateScaleMatrix(scale);    
}

Matrix4 Matrix4::CreateUnitVecRotMatrix(const Vec3& v, float angle)
{
    Matrix4 rotationMat = Identity();

    float c = cos(angle);
    float s = sin(angle);
    float t = 1 - cos(angle);

    rotationMat.Accessor(0, 0) = (t * v.x) * (t * v.x) + c;
    rotationMat.Accessor(1, 1) = (t * v.y) * (t * v.y) + c;
    rotationMat.Accessor(2, 2) = (t * v.z) * (t * v.z) + c;

    rotationMat.Accessor(0, 1) = t * v.x * v.y - s * v.z;
    rotationMat.Accessor(1, 2) = t * v.y * v.z - s * v.x;
    rotationMat.Accessor(2, 0) = t * v.z * v.x - s * v.y;
        
    rotationMat.Accessor(0, 2) = t * v.x * v.z + s * v.y;
    rotationMat.Accessor(1, 0) = t * v.x * v.y + s * v.z;
    rotationMat.Accessor(2, 1) = t * v.y * v.z + s * v.x;

    return rotationMat;
}

Matrix4 Matrix4::CreateProjMatrix(float d)
{
    Matrix4 projectionMat;

    projectionMat.Accessor(0, 0) = 1;
    projectionMat.Accessor(1, 1) = 1;
    projectionMat.Accessor(2, 2) = 1;
    projectionMat.Accessor(3, 2) = 1 / d;

    return projectionMat;
}

Matrix4 Matrix4::CreatePerspectiveProjectionMatrix(int width, int height, float near, float far, float fov)
{
    Matrix4 perspective;
    float temp, temp2, temp3, temp4;
    {
        float ymax, xmax;
        ymax = near * tanf(fov * 3.1415926f / 360.0f);
        xmax = ymax * (width / height);
        temp = 2 * near;
        temp2 = 2 * xmax;
        temp3 = 2 * ymax;
    }
    temp4 = far - near;
    perspective.matrix[0]  = temp / temp2;
    perspective.matrix[5]  = temp / temp3;
    perspective.matrix[10] = (-far - near) / temp4;
    perspective.matrix[11] = -1.0;
    perspective.matrix[14] = (-temp * far) / temp4;

    return perspective;
}

Matrix4 Matrix4::CreateOrthographicProjectionMatrix(int width, int height, float near, float far)
{
    Matrix4 orthographic = Identity();
        
    float aspect = (float)width / height;

    orthographic.matrix[0]  = 1.f;
    orthographic.matrix[5]  = 1.f / aspect;
    orthographic.matrix[10] = -2.f / (far - near);
    orthographic.matrix[12] = -1.f;
    orthographic.matrix[13] = -1.f;
    orthographic.matrix[14] = -(far + near) / (far - near);

    return orthographic;
}

Matrix4 Matrix4::Identity()
{
    return Matrix::Identity(4);
}

Matrix4 Matrix4::Zero()
{
    return Matrix::Zero(4, 4);
}

Vec3 Matrix4::operator*(const Vec3& v) const
{
    return {matrix[0] * v.x + matrix[4] * v.y + matrix[8] * v.z + matrix[12],
            matrix[1] * v.x + matrix[5] * v.y + matrix[9] * v.z + matrix[13],
            matrix[2] * v.x + matrix[6] * v.y + matrix[10] * v.z + matrix[14]};
}

Matrix4 Matrix4::operator*(const Matrix4& m) const
{
    Matrix4 result = Zero();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                result.Accessor(i, j) += this->Accessor(i, k) * m.Accessor(k, j);
            }
        }
    }

    return result;
}

void Matrix4::operator=(const Matrix4& m)
{
    for (int i = 0; i < 16; i++)
    {
        this->matrix[i] = m.matrix[i];
    }
}
