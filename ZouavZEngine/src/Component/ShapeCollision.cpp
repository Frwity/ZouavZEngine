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

ShapeCollision::ShapeCollision(GameObject* _gameObject, bool _isTrigger)
	 : Component(_gameObject), isTrigger(_isTrigger)
{
	
}

ShapeCollision::~ShapeCollision()
{

}

void ShapeCollision::releasePhysXComponent()
{

}

void ShapeCollision::AttachToRigidComponent()
{
	if (shape)
	{
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);

		RigidBody* rd = GetGameObject().GetComponent<RigidBody>();
		RigidStatic* rs = GetGameObject().GetComponent<RigidStatic>();

		if (rd)
		{
			ZASSERT(shape->getGeometryType() != physx::PxGeometryType::ePLANE, "Plane must be created with a RigidStatic");
			rd->actor->attachShape(*shape);
		}
		
		if (rs)
		{
			rs->actor->attachShape(*shape);
		}
	}
}