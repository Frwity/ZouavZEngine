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
	if(material->isReleasable())
		material->release();
	if(shape->isReleasable())
		shape->release();
	if(actor->isReleasable())
		actor->release();
}