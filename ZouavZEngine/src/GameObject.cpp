#include "GameObject.hpp"
#include "Scene.hpp"
#include "System/GameObjectSystem.hpp"

GameObject* GameObject::CreateGameObject()
{
	return GameObjectSystem::CreateGameObject();
}

GameObject::GameObject(GameObject* _parent)
	: parent(_parent)
{
}

