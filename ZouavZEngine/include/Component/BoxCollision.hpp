#pragma once

#include "Component/ShapeCollision.hpp"

class BoxCollision: public ShapeCollision
{
public:
	BoxCollision(GameObject* _gameObject, float _density = 1.0f);
	~BoxCollision();

};