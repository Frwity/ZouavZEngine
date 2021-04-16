#include "Component/RigidBody.hpp"
#include "GameObject.hpp"
#include "PxRigidBody.h"
#include "PxRigidDynamic.h"
#include "PxShape.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "extensions/PxSimpleFactory.h"
#include "extensions/PxRigidBodyExt.h"
#include "System/PhysicUtils.hpp"
#include "Component/BoxCollision.hpp"

using namespace physx;


RigidBody::RigidBody(GameObject* _gameObject)
	: Component(_gameObject)
{
	PxTransform t(PxVec3FromVec3(gameObject->WorldPosition()), PxQuatFromQuaternion(gameObject->WorldRotation()));

	actor = PhysicSystem::physics->createRigidDynamic(t);

	actor->userData = this;

	static int i = 0;
	if (i++ == 0)	
		actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	AttachShape();

	PhysicSystem::scene->addActor(*actor);
}

RigidBody::~RigidBody()
{
	//TODO detach shape and release actor
	//actor->release();
}

void RigidBody::SetLinearVelocity(class Vec3& v)
{
	actor->setLinearVelocity(PxVec3FromVec3(v));
}

Vec3 RigidBody::GetLinearVelocity()
{
	return Vec3FromPxVec3(actor->getLinearVelocity());
}

void RigidBody::DisableGravity()
{
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
}

void RigidBody::EnableGravity()
{
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
}

void RigidBody::AttachShape()
{
	//TODO use getComponents
	ShapeCollision* collision = gameObject->GetComponent<ShapeCollision>();

	if (collision && !collision->isAttach)
	{
		collision->isAttach = true;
		actor->attachShape(*collision->shape);
		//collision->shape->release();
	}
}

void RigidBody::OnContact()
{
	gameObject->OnContact();
}