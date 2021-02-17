#include "GameObject.hpp"
#include "Scene.hpp"
#include "System/GameObjectSystem.hpp"

GameObject* GameObject::CreateGameObject(const std::string& _name)
{
	return GameObjectSystem::CreateGameObject(_name);
}

GameObject::GameObject(const std::string& _name)
	: name(_name)
{
}