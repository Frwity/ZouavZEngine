#pragma once

#include "Maths/Vec3.hpp"
#include "Component/Transform.hpp"
#include "Component/Component.hpp"
#include "Rendering/MeshRenderer.hpp"
#include <vector>

class GameObject
{
private:
    std::vector<Component> Components;

public:
    Transform transform;
    MeshRenderer meshRenderer;

    GameObject() = default;
    GameObject(const Transform& _transform, const MeshRenderer& _meshRenderer);
    ~GameObject() = default;

    void AddChild(GameObject& _child);
};