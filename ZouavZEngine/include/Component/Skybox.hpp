#pragma once

#include "Component.hpp"

class Skybox : public Component
{
private:
	unsigned int texture;
	std::shared_ptr<class Shader> shader;
	std::shared_ptr<class Mesh> cubeMesh;

public:
	Skybox() = delete;
	Skybox(class GameObject* _gameObject);
	Skybox(class GameObject* _gameObject, const char* _texturePath);
	Skybox(const Skybox& _other);
	Component* Clone() const override { return new Skybox(*this); }
	~Skybox() = default;

	void Draw(const class Camera& _camera);

	void Editor() override;

	const char* GetComponentName() override { return "Skybox"; }
};