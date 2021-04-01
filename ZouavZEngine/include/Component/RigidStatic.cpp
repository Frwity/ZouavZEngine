#include "Component/RigidStatic.hpp"
#include "GameObject.hpp"
#include "PxRigidStatic.h"
#include "System/PhysicUtils.hpp"
#include "Component/BoxCollision.hpp"
#include "PxScene.h"
#include "System/PhysicSystem.hpp"

using namespace physx;

RigidStatic::RigidStatic(GameObject* _gameObject)
	: Component(_gameObject)
{
	PxTransform t(PxVec3FromVec3(gameObject->WorldPosition()), PxQuatFromQuaternion(gameObject->WorldRotation()));

	actor = PhysicSystem::physics->createRigidStatic(t);

	actor->userData = gameObject;

	AttachShape();

	PhysicSystem::scene->addActor(*actor);
}

void RigidStatic::AttachShape()
{
	//TODO use getComponents
	ShapeCollision* collision = gameObject->GetComponent<ShapeCollision>();

	if (collision && !collision->isAttach)
	{
		collision->isAttach = true;
		actor->attachShape(*collision->shape);
		collision->shape->release();
	}
}

RigidStatic::~RigidStatic()
{
	//actor->release();
}