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

void Transform::Translate(const Vec3& _direction)
{
	position += _direction;
}

void Transform::Rotate(const Quaternion& _rotToAdd)
{
	rotation = rotation * _rotToAdd;
}

Vec3 Transform::Right()
{
	return rotation.RotateVector(Vec3::Right());
}

Vec3 Transform::Up()
{
	return rotation.RotateVector(Vec3::Up());
}

Vec3 Transform::Forward()
{
	return rotation.RotateVector(Vec3::Forward());
}