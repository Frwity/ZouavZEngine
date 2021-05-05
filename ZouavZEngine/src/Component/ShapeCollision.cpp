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
#include "System/PhysicUtils.hpp"

ShapeCollision::ShapeCollision(GameObject* _gameObject/*, Transform _transform*/, bool _isTrigger)
	 : Component(_gameObject), isTrigger(_isTrigger)
{
	//transform = _transform;
}

ShapeCollision::~ShapeCollision()
{

}

void ShapeCollision::releasePhysXComponent()
{

}

void ShapeCollision::UpdateIsTrigger()
{
	shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
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

void ShapeCollision::UpdateTransform(class Transform& _transform)
{
	transform = _transform;

	if (shape)
		shape->setLocalPose(PxTransformFromTransform(transform));
}