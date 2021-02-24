#include "GameObject.hpp"
#include "Scene.hpp"
#include "Maths/Mat4.hpp"
#include "System/GameObjectSystem.hpp"

GameObject* GameObject::CreateGameObject(const std::string& _name)
{
	return GameObjectSystem::CreateGameObject(_name);
}

GameObject::GameObject(const std::string& _name)
	: name(_name)
{
}

void GameObject::UpdateTransform(const Mat4& _heritedTransform)
{
	worldPosition = _heritedTransform * localPosition;
	worldRotation = parent ? parent->worldRotation * localRotation : localRotation;
	worldScale = parent ? parent->worldScale * localScale : localScale;

	for (GameObject* _child : children)
	{
		_child->UpdateTransform(_heritedTransform * Mat4::CreateTRSMatrix(localPosition, localRotation, localScale));
	}
}

bool GameObject::IsChildOf(const GameObject* _gameObject) const
{
	if (!parent)
		return false;

	if (parent == _gameObject)
		return true;

	return parent->IsChildOf(_gameObject);
}