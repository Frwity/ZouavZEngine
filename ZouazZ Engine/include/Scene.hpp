#pragma once
#include "GameObject.hpp"
#include <vector>

class Scene
{
private:
	std::vector<GameObject> children;

public:
	Scene() = default;
	~Scene() = default;

	GameObject& AddGameObject();

	void Draw();
};