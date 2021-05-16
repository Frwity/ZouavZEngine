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
#include "imgui.h"

using namespace physx;


BoxCollision::BoxCollision(GameObject* _gameObject, Vec3 _halfExtends, bool _isTrigger)
	: ShapeCollision(_gameObject, _isTrigger), halfExtends(_halfExtends)
{
	shape = PhysicSystem::physics->createShape(PxBoxGeometry(PxVec3FromVec3(_halfExtends)), *material);

	shape->userData = &GetGameObject(); // TODO is it realy usefull ?

	AttachToRigidComponent();
}

BoxCollision::BoxCollision(const BoxCollision& _other)
	: ShapeCollision(_other), halfExtends{ _other.halfExtends }
{
	shape = PhysicSystem::physics->createShape(PxBoxGeometry(PxVec3FromVec3(_other.halfExtends)), *material);

	shape->userData = &GetGameObject(); // TODO is it realy usefull ?

	AttachToRigidComponent();
}

BoxCollision::~BoxCollision()
{
	
}

void BoxCollision::Editor()
{
	ImGui::SliderFloat3("Half Extends : ", &halfExtends.x, 0.0f, 100.0f);
}