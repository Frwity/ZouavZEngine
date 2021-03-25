#include "Scene.hpp"
#include "Maths/Mat4.hpp"
#include "System/GameObjectSystem.hpp"
#include "Component/RigidBody.hpp"
#include "Component/RigidStatic.hpp"
#include "System/PhysicUtils.hpp"
#include "PxRigidDynamic.h"
#include "PxRigidStatic.h"
#include "PxActor.h"
#include "GameObject.hpp"

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
