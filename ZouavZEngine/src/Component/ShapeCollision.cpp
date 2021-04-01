#include "Component/ShapeCollision.hpp"
#include "PxActor.h"
#include "PxMaterial.h"
#include "PxShape.h"
#include "PxRigidDynamic.h"
#include "PxRigidStatic.h"
#include "GameObject.hpp"
#include "Component/RigidBody.hpp"
#include "Component/RigidStatic.hpp"
#include "System/Debug.hpp"

ShapeCollision::ShapeCollision(GameObject* _gameObject)
	 : Component(_gameObject)
{
	
}

ShapeCollision::~ShapeCollision()
{
	//TODO fix destruction
	//shape->release();
	//material->release();
}

void ShapeCollision::releasePhysXComponent()
{

}

void ShapeCollision::AttachToRigidComponent()
{
	if (shape)
	{
		RigidBody* rd = gameObject->GetComponent<RigidBody>();
		RigidStatic* rs = gameObject->GetComponent<RigidStatic>();

		if (rd)
		{
			ZASSERT(shape->getGeometryType() == physx::PxGeometryType::ePLANE, "Plane must created with a RigidStatic");
			rd->actor->attachShape(*shape);
		}
		
		if (rs)
			rs->actor->attachShape(*shape);
	}
}