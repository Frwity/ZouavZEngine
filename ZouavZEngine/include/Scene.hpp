#pragma once
#include "GameObject.hpp"
#include <vector>

class Scene
{
private:
	static Scene* currentScene;

	GameObject world{ "World" };

	void DrawChild(GameObject* _parent, const Mat4& _heritedMatrix, const class Camera& _camera) const;
	
	std::vector<class Light*> lights;

public:

	Scene();
	~Scene() = default;

	static Scene* GetCurrentScene() { return currentScene; }
	GameObject& GetWorld() { return world; }

	void Load();
	void Save();

	void Draw(const class Camera& _camera) const;

	void AddLight(class Light* _newLight);
	const std::vector<class Light*>& GetLights() { return lights; }
};
