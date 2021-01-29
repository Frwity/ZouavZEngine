#pragma once

#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Texture.hpp"
#include "Component/Component.hpp"

class MeshRenderer : public Component
{
private:
    Mesh* mesh;
    Texture* texture;

public:
    Shader* shader;

    MeshRenderer() = default;
    MeshRenderer(class GameObject* _gameObject, Mesh* _mesh, Shader* _shader, Texture* _texture);

    ~MeshRenderer() = default;

    void Draw(const class Mat4& _parent, const class Camera& _camera);
};