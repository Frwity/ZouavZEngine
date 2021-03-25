#pragma once

#include "Component/ShapeCollision.hpp"

class CapsuleCollision: public ShapeCollision
{
public:
	float halfHeight;
	float radius;

	CapsuleCollision(GameObject* _gameObject, float _radius = 1.0f, float _halfHeight = 1.0f);
	~CapsuleCollision();

};