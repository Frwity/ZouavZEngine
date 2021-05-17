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
#include "Component/ShapeCollision.hpp"
#include "Component/ScriptComponent.hpp"
#include "imgui.h"

using namespace physx;


RigidBody::RigidBody(GameObject* _gameObject)
	: Rigid(_gameObject)
{
	PxTransform t(PxVec3FromVec3(GetGameObject().WorldPosition()), PxQuatFromQuaternion(GetGameObject().WorldRotation()));

	actor = PhysicSystem::physics->createRigidDynamic(t);

	actor->userData = this;

	AttachShape();

	PhysicSystem::scene->addActor(*actor);

	if (!_gameObject->IsActive())
		InternalDehactivate();
	
	LockAxis();
}

RigidBody::RigidBody(const RigidBody& _other)
	: Component(_other)
{
	PxTransform t(PxVec3FromVec3(GetGameObject().WorldPosition()), PxQuatFromQuaternion(GetGameObject().WorldRotation()));

	actor = PhysicSystem::physics->createRigidDynamic(t);

	actor->userData = this;

	AttachShape();

	PhysicSystem::scene->addActor(*actor);

	if (!_other.IsActive())
		InternalDehactivate();
	LockAxis();
}

RigidBody::~RigidBody()
{
	
}

void RigidBody::SetLinearVelocity(const class Vec3& v)
{
	((PxRigidBody*)actor)->setLinearVelocity(PxVec3FromVec3(v));
}

Vec3 RigidBody::GetLinearVelocity()
{
	return Vec3FromPxVec3(((PxRigidBody*)actor)->getLinearVelocity());
}

void RigidBody::DisableGravity()
{
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
}

void RigidBody::EnableGravity()
{
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
}

void RigidBody::Editor()
{
	if (ImGui::Checkbox("Lock Rotaion X ", &lockX))
		LockAxis();

	if (ImGui::Checkbox("Lock Rotaion Y ", &lockY))
		LockAxis();

	if (ImGui::Checkbox("Lock Rotaion Z ", &lockZ))
		LockAxis();
}

template <class Archive>
static void RigidBody::load_and_construct(Archive& _ar, cereal::construct<RigidBody>& _construct)
{
	_construct(GameObject::currentLoadedGameObject);
}

void RigidBody::Activate()
{
	Component::Activate();
	actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
}

void RigidBody::Dehactivate()
{
	Component::Dehactivate();
	actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
}

void RigidBody::InternalActivate()
{
	if (isActive)
		actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
}

void RigidBody::InternalDehactivate()
{
	if (isActive)
		actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
}

void RigidBody::Editor()
{
	if (ImGui::Checkbox("Lock Rotaion X ", &lockX))
        LockAxis();

    if (ImGui::Checkbox("Lock Rotaion Y ", &lockY))
        LockAxis();

    if (ImGui::Checkbox("Lock Rotaion Z ", &lockZ))
        LockAxis();
}

void RigidBody::LockAxis()
{
	PxRigidDynamic* rd = static_cast<PxRigidDynamic*>(actor);
}

template <class Archive>
static void RigidBody::load_and_construct(Archive& _ar, cereal::construct<RigidBody>& _construct)
{
	_construct(GameObject::currentLoadedGameObject);
	_ar(cereal::base_class<Component>(_construct.ptr()));
	rd->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, lockZ);
	rd->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, lockZ);
	rd->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, lockY);
}