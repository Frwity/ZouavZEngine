#include "Component/Transform.hpp"


Transform::Transform(const Vec3& _position, const Vec3& _rotation, const Vec3& _scale)
	 : Component(nullptr), parent(nullptr), position(_position), rotation(_rotation), scale(_scale)
{
}

Transform::Transform(const Vec3& _position, const Quaternion& _rotation, const Vec3& _scale)
	: Component(nullptr), parent(nullptr), position(_position), rotation(_rotation), scale(_scale)
{
}

void Transform::AddChild(Transform* _child) 
{
	children.emplace_back(_child);
	_child->parent = this;
}