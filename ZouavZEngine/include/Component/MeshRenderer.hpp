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
	friend class Engine;
	static Mesh* defaultMesh;
	static Texture* defaultTexture;
	static Shader* defaultShader;

public:
    Mesh* mesh = nullptr;
    Texture* texture = nullptr;
    Shader* shader = nullptr;

    MeshRenderer() = delete;
    MeshRenderer(class GameObject* _gameObject);
    MeshRenderer(class GameObject* _gameObject, Mesh* _mesh, Texture* _texture, Shader* _shader);

    ~MeshRenderer() = default;

    void Draw(const class Mat4& _transform, const class Camera& _camera);

    void Editor() override;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(mesh->GetName(), texture->GetName(), shader->GetName());
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<MeshRenderer>& _construct)
	{
		std::string meshName;
		std::string textureName;
		std::string shaderName;

		_ar(meshName, textureName, shaderName);

		_construct(nullptr, ResourcesManager::GetResource<Mesh>(meshName),
							ResourcesManager::GetResource<Texture>(textureName),
							ResourcesManager::GetResource<Shader>(shaderName));
	}
};

CEREAL_REGISTER_TYPE(MeshRenderer)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, MeshRenderer)
