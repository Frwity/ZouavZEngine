#include "Component/ShapeCollision.hpp"
#include "PxActor.h"

ShapeCollision::ShapeCollision(GameObject* _gameObject)
	 : Component(_gameObject)
{
	
}

ShapeCollision::~ShapeCollision()
{
	actor->release();
}