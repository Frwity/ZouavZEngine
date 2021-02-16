#pragma once

#include "Maths/Vec3.hpp"
#include "Maths/Quaternion.hpp"
#include <vector>

class Transform
{
public:

    Vec3       position{0.0f, 0.0f, 0.0f};
    Quaternion rotation{1.0f, 0.0f, 0.0f, 0.0f};
    Vec3       scale{1.0f, 1.0f, 1.0f};

    Transform() = default;
    Transform(const Vec3& _position, const Vec3& _rotation, const Vec3& _scale);
    Transform(const Vec3& _position, const Quaternion& _rotation, const Vec3& _scale);
    ~Transform() = default;

    static Transform InitTransform();

    void Translate(const Vec3& _direction);
    void Rotate(const Quaternion& _rotToAdd);

    Vec3 Right();
    Vec3 Up();
    Vec3 Forward();
};