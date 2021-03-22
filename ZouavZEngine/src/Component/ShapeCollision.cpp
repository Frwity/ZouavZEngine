#include "Component/ShapeCollision.hpp"
#include "PxActor.h"
#include "PxMaterial.h"
#include "PxShape.h"

ShapeCollision::ShapeCollision(GameObject* _gameObject)
	 : Component(_gameObject)
{
	
}

ShapeCollision::~ShapeCollision()
{

}

void ShapeCollision::releasePhysXComponent()
{
	//shape->release();
	material->release();
	actor->release();
}