#include "Component/Transform.hpp"
#include "GameObject.hpp"


GameObject::GameObject(const Transform& _transform, const MeshRenderer& _meshRenderer)
	: transform(_transform), meshRenderer(_meshRenderer)
{
	transform.gameObject = this;
	meshRenderer.gameObject = this;
}

void GameObject::AddChild(GameObject& _child) 
{
	transform.AddChild(&_child.transform);
}
