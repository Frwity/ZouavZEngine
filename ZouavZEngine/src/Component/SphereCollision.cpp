#include "Component/SphereCollision.hpp"
#include "GameObject.hpp"
#include "PxRigidBody.h"
#include "PxRigidStatic.h"
#include "PxShape.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "extensions/PxSimpleFactory.h"
#include "System/PhysicUtils.hpp"

using namespace physx;

SphereCollision::SphereCollision(GameObject* _gameObject, float _radius, float _density)
	: ShapeCollision(_gameObject, _density), radius(_radius)
{
	PxTransform t(PxVec3FromVec3(gameObject->WorldPosition()), PxQuatFromQuaternion(gameObject->WorldRotation()));

	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);
	
	actor = PxCreateStatic(*PhysicSystem::physics, t, PxSphereGeometry(radius), *material);

	actor->userData = gameObject;

	PhysicSystem::scene->addActor(*actor);
}