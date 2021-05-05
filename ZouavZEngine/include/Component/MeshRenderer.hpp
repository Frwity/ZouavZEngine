#pragma once

#include "Rendering/Mesh.hpp"
#include "Rendering/Material.hpp"
#include "Component/Component.hpp"
#include "System/ResourcesManager.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"


class MeshRenderer : public Component
{
private:
	friend class Engine;

public:
	Mesh* mesh = nullptr;
	Material material;

	MeshRenderer() = delete;
	MeshRenderer(class GameObject* _gameObject);
	MeshRenderer(class GameObject* _gameObject, Mesh* _mesh, Texture* _texture, Shader* _shader);

	~MeshRenderer() = default;

	void Draw(const class Mat4& _transform, const class Camera& _camera);

	void TextureEditor();
	void ShaderEditor();
	void MeshEditor();
	void Editor() override;

	const char* GetComponentName() override { return "MeshRenderer"; }

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(mesh->GetName(), mesh->GetPaths()[0], 
			material.texture->GetName(), material.texture->GetPaths()[0], 
			material.shader->GetName(), material.shader->GetPaths()[0], material.shader->GetPaths()[1]);
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<MeshRenderer>& _construct);
};

CEREAL_REGISTER_TYPE(MeshRenderer)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, MeshRenderer)
