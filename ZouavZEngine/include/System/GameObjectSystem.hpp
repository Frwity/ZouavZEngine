#pragma once
#include "GameObject.hpp"
#include <vector>
#include <memory>

class GameObjectSystem
{
private:
	static std::vector<std::unique_ptr<GameObject>> gameObjects;
public:

	GameObjectSystem() = delete;
	~GameObjectSystem() = delete;

	static GameObject* CreateGameObject();
};