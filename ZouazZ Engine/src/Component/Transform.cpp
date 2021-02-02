#include "Component/Transform.hpp"


Transform::Transform(const Vec3& _position, const Vec3& _rotation, const Vec3& _scale)
	 : position(_position), rotation(_rotation), scale(_scale)
{
}

Transform::Transform(const Vec3& _position, const Quaternion& _rotation, const Vec3& _scale)
	: position(_position), rotation(_rotation), scale(_scale)
{
}

Transform Transform::InitTransform()
{
	return Transform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
}