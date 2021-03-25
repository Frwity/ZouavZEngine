#pragma once

#include "Component/ShapeCollision.hpp"

class BoxCollision: public ShapeCollision
{
public:
	BoxCollision(GameObject* _gameObject);
	~BoxCollision();

};