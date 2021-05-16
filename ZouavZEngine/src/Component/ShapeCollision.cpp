#include "Component/ShapeCollision.hpp"
#include "PxActor.h"
#include "PxMaterial.h"
#include "PxShape.h"
#include "PxRigidDynamic.h"
#include "PxRigidStatic.h"
#include "GameObject.hpp"
#include "Component/RigidBody.hpp"
#include "Component/RigidStatic.hpp"
#include "System/PhysicSystem.hpp"
#include "System/Debug.hpp"

ShapeCollision::ShapeCollision(GameObject* _gameObject, bool _isTrigger)
	 : Component(_gameObject), isTrigger(_isTrigger)
{
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);

	if (!_gameObject->IsActive())
		InternalDehactivate();
}

ShapeCollision::ShapeCollision(const ShapeCollision& _other)
	: Component(_other)
{
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f); // TODO physxsystem material manager
	isTrigger = _other.isTrigger;
	if (!_other.IsActive())
		InternalDehactivate();
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
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);

		RigidBody* rd = GetGameObject().GetComponent<RigidBody>();
		RigidStatic* rs = GetGameObject().GetComponent<RigidStatic>();

		if (rd)
		{
			ZASSERT(shape->getGeometryType() != physx::PxGeometryType::ePLANE, "Plane must be created with a RigidStatic");
			rd->actor->attachShape(*shape);
		}
		
		if (rs)
			rs->actor->attachShape(*shape);
	}
}


void ShapeCollision::Activate()
{
	Component::Activate();
	shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
}

void ShapeCollision::Dehactivate()
{
	Component::Dehactivate();
	shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
}

void ShapeCollision::InternalActivate()
{
	if (isActive)
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
}

void ShapeCollision::InternalDehactivate()
{
	if (isActive)
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
}