#include "GameObject.hpp"

GameObject::GameObject()
	: transform(Transform::InitTransform())
{
	transform.gameObject = this;
}

void GameObject::AddChild(GameObject& _child) 
{
	transform.AddChild(&_child.transform);
} 
