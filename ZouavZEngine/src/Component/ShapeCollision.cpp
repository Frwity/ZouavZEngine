#include "Component/ShapeCollision.hpp"
#include "PxActor.h"
#include "PxMaterial.h"
#include "PxShape.h"

ShapeCollision::ShapeCollision(GameObject* _gameObject, float _density)
	 : Component(_gameObject), density(_density)
{
	
}

ShapeCollision::~ShapeCollision()
{

}

void ShapeCollision::releasePhysXComponent()
{
	material->release();
	actor->release();
}