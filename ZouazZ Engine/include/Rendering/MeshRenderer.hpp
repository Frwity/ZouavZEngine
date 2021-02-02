#pragma once

#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Texture.hpp"
#include "Component/Component.hpp"

class MeshRenderer : public Component
{
private:
    Mesh* mesh;
    Shader* shader;
    Texture* texture;

public:

    MeshRenderer() = default;
    MeshRenderer(class GameObject* _gameObject, Mesh* _mesh, Shader* _shader, Texture* _texture);

    ~MeshRenderer() = default;

    void Draw(const class Mat4& _transform, const class Camera& _camera);
};