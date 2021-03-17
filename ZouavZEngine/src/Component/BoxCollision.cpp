#include "Component/BoxCollision.hpp"
#include "GameObject.hpp"
#include "PxActor.h"
#include "PxRigidDynamic.h"
#include "PxRigidStatic.h"
#include "PxShape.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "extensions/PxRigidBodyExt.h"

using namespace physx;

static PxVec3 PxVec3FromVec3(Vec3 v) { return { v.x, v.y, v.z }; }
static PxQuat PxQuatFromQuaternion(Quaternion q) { return { q.x, q.y, q.z, q.w }; }

BoxCollision::BoxCollision(GameObject* _gameObject)
	: ShapeCollision(_gameObject)
{
	actor = PhysicSystem::physics->createRigidDynamic(PxTransform(PxVec3FromVec3(gameObject->WorldPosition()), PxQuatFromQuaternion(gameObject->WorldRotation())));
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);
	shape = PhysicSystem::physics->createShape(PxBoxGeometry(0.5f,0.5f,0.5f), *material);

	PxRigidBodyExt::updateMassAndInertia(*static_cast<PxRigidDynamic*>(actor), 1.0f);

	actor->userData = gameObject;
	
	PhysicSystem::scene->addActor(*actor);
}

BoxCollision::~BoxCollision()
{
	
}