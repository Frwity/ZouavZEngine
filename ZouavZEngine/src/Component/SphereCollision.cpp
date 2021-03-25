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

SphereCollision::SphereCollision(GameObject* _gameObject, float _radius)
	: ShapeCollision(_gameObject), radius(_radius)
{
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);
	
	shape = PhysicSystem::physics->createShape(PxSphereGeometry(0.5f), *material);

	//attach shape to rigidbody or rigidstatic if exist
	AttachToRigidComponent();
}

SphereCollision::~SphereCollision()
{

}