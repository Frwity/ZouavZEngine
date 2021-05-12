#pragma once

#include "Component.hpp"

class Skybox : public Component
{
private:
	unsigned int texture;
	class Shader* shader;
	class Mesh* cubeMesh;

public:
	Skybox() = delete;
	Skybox(class GameObject* _gameObject);
	Skybox(class GameObject* _gameObject, const char* _texturePath);
	~Skybox() = default;

	void Draw(const class Camera& _camera);

	void Editor() override;

	const char* GetComponentName() override { return "Skybox"; }
};