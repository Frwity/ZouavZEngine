#include "Scene.hpp"
#include "Maths/Mat4.hpp"
#include "Component/RigidBody.hpp"
#include "Component/RigidStatic.hpp"
#include "System/PhysicUtils.hpp"
#include "PxRigidDynamic.h"
#include "PxRigidStatic.h"
#include "PxActor.h"
#include "GameObject.hpp"

bool GameObject::destroyGameObject = false;
GameObject* GameObject::currentLoadedGameObject = nullptr;
std::vector<std::unique_ptr<GameObject>> GameObject::gameObjects;

GameObject* GameObject::CreateGameObject(const std::string& _name)
{
	gameObjects.emplace_back(std::make_unique<GameObject>(_name));
	Scene::GetCurrentScene()->GetWorld().AddChild(gameObjects.back().get());
	return gameObjects.back().get();
}

GameObject::GameObject(const std::string& _name)
	: name(_name)
{
}

void GameObject::Destroy() 
{ 
	toDestroy = true;
}

void GameObject::UpdateTransform(const Mat4& _heritedTransform)
{
	worldPosition = _heritedTransform * localPosition;
	worldRotation = parent ? parent->worldRotation * localRotation : localRotation;
	worldScale = parent ? parent->worldScale * localScale : localScale;

	for (GameObject* _child : children)
	{
		_child->UpdateTransform(_heritedTransform * Mat4::CreateTRSMatrix(localPosition, localRotation, localScale));
		
		RigidBody* rd = _child->GetComponent<RigidBody>();

		//update physx transform for simulation
		if (rd)
			rd->actor->setGlobalPose(PxTransformFromTransform(static_cast<Transform>(*_child)));

		RigidStatic* rs = _child->GetComponent<RigidStatic>();

		if (rs)
			rs->actor->setGlobalPose(PxTransformFromTransform(static_cast<Transform>(*_child)));
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

void GameObject::AddChild(GameObject* _child)
{
	children.push_back(_child);
	if (children.back()->parent)
		children.back()->parent->RemoveChild(children.back());
	children.back()->parent = this;
}

void GameObject::SetParent(GameObject* _parent)
{
	if (parent)
		parent->RemoveChild(this);

	parent = _parent;

	if (_parent)
		_parent->children.push_back(this);
}

void GameObject::RemoveChild(GameObject* _child)
{
	for (auto it = children.begin(); it != children.end(); )
	{
		if (*it == _child)
			it = children.erase(it);
		else
			++it;
	}
}

const std::vector<std::unique_ptr<Component>>& GameObject::GetComponents()
{
	return components;
}

void GameObject::DestroyGameObjectIfNeedTo()
{
	if (destroyGameObject)
	{
		for (auto iter = gameObjects.begin(); iter != gameObjects.end();)
		{
			if (iter->get()->toDestroy)
			{
				iter = gameObjects.erase(iter);
			}
			else
				iter++;
		}
	}
}

GameObject* GameObject::GetGameObjectByName(std::string _name)
{
	for (std::unique_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->name == _name)
			return gameObject.get();
	}

	return nullptr;
}

std::vector<GameObject*> GameObject::GetGameObjectsByName(std::string _name)
{
	std::vector<GameObject*> toReturn;

	for (std::unique_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->name == _name)
			toReturn.push_back(gameObject.get());
	}

	return toReturn;
}

GameObject* GameObject::GetGameObjectByTag(std::string _tag)
{
	for (std::unique_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->tag == _tag)
			return gameObject.get();
	}

	return nullptr;
}

std::vector<GameObject*> GameObject::GetGameObjectsByTag(std::string _tag)
{
	std::vector<GameObject*> toReturn;

	for (std::unique_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->tag == _tag)
			toReturn.push_back(gameObject.get());
	}

	return toReturn;
}

void GameObject::OnContact()
{

}