#pragma once

#include "Maths/Vec3.hpp"
#include "Maths/Quaternion.hpp"
#include "Component/Component.hpp"
#include <vector>

class Transform : public Component
{
private:
    Transform* parent{nullptr};
    std::vector<Transform*> children;

public:
    Vec3       position{0.0f, 0.0f, 0.0f};
    Quaternion rotation{1.0f, 0.0f, 0.0f, 0.0f};
    Vec3       scale{1.0f, 1.0f, 1.0f};

    Transform() = default;
    Transform(const Vec3& _position, const Vec3& _rotation, const Vec3& _scale);
    Transform(const Vec3& _position, const Quaternion& _rotation, const Vec3& _scale);
    ~Transform() = default;

    void AddChild(Transform* _child);

    Transform* GetParent() const { return parent; }

    static Transform InitTransform();
};