#include "Component/BoxCollision.hpp"
#include "GameObject.hpp"
#include "PxActor.h"
#include "PxRigidBody.h"
#include "PxRigidStatic.h"
#include "PxShape.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "extensions/PxSimpleFactory.h"
#include "extensions/PxRigidBodyExt.h"
#include "System/PhysicUtils.hpp"

using namespace physx;


BoxCollision::BoxCollision(GameObject* _gameObject)
	: ShapeCollision(_gameObject)
{
	PxTransform t(PxVec3FromVec3(gameObject->WorldPosition()), PxQuatFromQuaternion(gameObject->WorldRotation()));

	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);

	//temp
	Vec3 v{ 0.5f, 0.5f, 0.5f };
	
	shape = PhysicSystem::physics->createShape(PxBoxGeometry(PxVec3FromVec3(v)), *material);
	shape->userData = gameObject;

	AttachToRigidComponent();
}

BoxCollision::~BoxCollision()
{
	
}