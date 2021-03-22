#pragma once

#include "Component/ShapeCollision.hpp"

class SphereCollision : public ShapeCollision
{
public:
	float radius;

	SphereCollision(GameObject* _gameObject, float radius = 1.0f, float density = 1.0f);
	~SphereCollision() {};
};