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


BoxCollision::BoxCollision(GameObject* _gameObject, Vec3 _halfExtends, bool _isTrigger)
	: ShapeCollision(_gameObject, _isTrigger)
{
	PxTransform t(PxVec3FromVec3(gameObject->WorldPosition()), PxQuatFromQuaternion(gameObject->WorldRotation()));

	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);
	
	shape = PhysicSystem::physics->createShape(PxBoxGeometry(PxVec3FromVec3(_halfExtends)), *material);

	shape->userData = gameObject;

	AttachToRigidComponent();
}

BoxCollision::~BoxCollision()
{
	
}