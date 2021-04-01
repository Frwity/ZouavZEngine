#pragma once

#include "Component/ShapeCollision.hpp"

class Plane: public ShapeCollision
{
public:
	Plane() = delete;
	Plane(GameObject* _gameObject);
	~Plane();

private:

};