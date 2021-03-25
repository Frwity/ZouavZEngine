#pragma once

#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Texture.hpp"
#include "Component/Component.hpp"
#include "System/ResourcesManager.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"


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

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(mesh->GetName(), texture->GetName(), shader->GetName());
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<MeshRenderer>& _construct)
	{
		std::string meshName;
		std::string shaderName;
		std::string textureName;

		_ar(meshName, shaderName, textureName);

		_construct(nullptr, ResourcesManager::GetResource<Mesh>(meshName),
							ResourcesManager::GetResource<Shader>(shaderName),
							ResourcesManager::GetResource<Texture>(textureName));
	}
};

CEREAL_REGISTER_TYPE(MeshRenderer)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, MeshRenderer)