#include "System/GameObjectSystem.hpp"
#include "Scene.hpp"

std::vector<std::unique_ptr<GameObject>> GameObjectSystem::gameObjects;

GameObject* GameObjectSystem::CreateGameObject()
{
	gameObjects.emplace_back(std::make_unique<GameObject>(nullptr));
	Scene::GetCurrentScene()->GetWorld().AddChild(gameObjects.back().get());
	return gameObjects.back().get();
}