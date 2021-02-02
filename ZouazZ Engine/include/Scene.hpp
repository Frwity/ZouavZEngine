#pragma once
#include "GameObject.hpp"
#include <vector>

class Mat4;

class Scene
{
private:
	static Scene* currentScene;

	GameObject world{ nullptr };

	void DrawChild(GameObject* _parent, const Mat4& _heritedMatrix) const;

public:
	std::vector<class Light*> lights;

	Scene();
	~Scene() = default;

	static Scene* GetCurrentScene() { return currentScene; }
	GameObject& GetWorld() { return world; }

	void Draw() const;
};