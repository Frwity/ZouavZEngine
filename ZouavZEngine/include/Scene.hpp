#pragma once
#include "GameObject.hpp"
#include <vector>

class Mat4;

class Scene
{
private:
	static Scene* currentScene;

	GameObject world{ "World" };

	void DrawChild(GameObject* _parent, const Mat4& _heritedMatrix, const class Camera& _camera) const;

public:
	std::vector<class Light*> lights;

	Scene();
	~Scene() = default;

	static Scene* GetCurrentScene() { return currentScene; }
	GameObject& GetWorld() { return world; }

	void Draw(const class Camera& _camera) const;
	void SimulatePhyics() const;					//Simulate phyics and update transform of affected gameObjects
};