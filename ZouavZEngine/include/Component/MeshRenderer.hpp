#pragma once

#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Texture.hpp"
#include "Component/Component.hpp"

class MeshRenderer : public Component
{
private:

public:
    Mesh* mesh = nullptr;
    Texture* texture = nullptr;
    Shader* shader = nullptr;

    MeshRenderer() = delete;
    MeshRenderer(class GameObject* _gameObject);
    MeshRenderer(class GameObject* _gameObject, Mesh* _mesh, Shader* _shader, Texture* _texture);

    ~MeshRenderer() = default;

    void Draw(const class Mat4& _transform, const class Camera& _camera);

    void Editor() override;
};