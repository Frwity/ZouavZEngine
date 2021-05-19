#include "Component/Rigid.hpp"
#include "GameObject.hpp"
#include "PxRigidBody.h"
#include "PxRigidDynamic.h"
#include "PxShape.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "extensions/PxSimpleFactory.h"
#include "extensions/PxRigidActorExt.h"
#include "extensions/PxRigidBodyExt.h"
#include "System/PhysicUtils.hpp"
#include "Component/ShapeCollision.hpp"

using namespace physx;

Rigid::Rigid(GameObject* _gameObject)
	: Component(_gameObject)
{

}

Rigid::Rigid(const Rigid& _other)
	: Component(_other)
{

}

void Rigid::Activate()
{
	Component::Activate();
	actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
}

void Rigid::Dehactivate()
{
	Component::Dehactivate();
	actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
}

void Rigid::InternalActivate()
{
	if (isActive)
		actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
}

void Rigid::InternalDehactivate()
{
	if (isActive)
		actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
}

void Rigid::AttachShape()
{
	std::vector<ShapeCollision*> collisions = GetGameObject().GetComponents<ShapeCollision>();

	for (ShapeCollision* collision : collisions)
	{
		if (collision && !collision->isAttach)
		{
			collision->isAttach = true;

			collision->shape = physx::PxRigidActorExt::createExclusiveShape(*actor, *collision->geometry, *collision->material);
			if(!collision->shape)
				Debug::LogError("Attach to shape failed !");
		}
	}
}

void Rigid::OnContact(GameObject* _other, physx::PxShape* _collidingShape)
{
	if (gameObject == nullptr)
		return;

	ScriptComponent* script = GetGameObject().GetComponent<ScriptComponent>();

	ShapeCollision* collision = static_cast<ShapeCollision*>(_collidingShape->userData);

	if (script)
		script->OnContact(_other, collision);
}

void Rigid::OnTrigger(GameObject* _other, physx::PxShape* _collidingShape)
{
	if (gameObject == nullptr)
		return;

	ScriptComponent* script = GetGameObject().GetComponent<ScriptComponent>();

	ShapeCollision* collision = static_cast<ShapeCollision*>(_collidingShape->userData);

	if (script)
		script->OnTrigger(_other, collision);
}

Rigid::~Rigid()
{
	if (!PhysicSystem::scene)
		return;

	if (actor)
		PhysicSystem::scene->removeActor(*actor);
}