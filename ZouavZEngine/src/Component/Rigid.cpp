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
	//TODO use getComponents
	ShapeCollision* collision = GetGameObject().GetComponent<ShapeCollision>();

	if (collision && !collision->isAttach)
	{
		collision->isAttach = true;
		if (!actor->attachShape(*collision->shape))
			std::cout << "Attach to shape failed !" << std::endl;
		collision->shape->release();
	}
}

void Rigid::OnContact(GameObject* _other)
{
	if (gameObject == nullptr)
		return;

	ScriptComponent* script = GetGameObject().GetComponent<ScriptComponent>();

	if (script)
		script->OnContact(_other);
}

void Rigid::OnTrigger(GameObject* _other)
{
	if (gameObject == nullptr)
		return;

	ScriptComponent* script = GetGameObject().GetComponent<ScriptComponent>();

	if (script)
		script->OnTrigger(_other);
}

Rigid::~Rigid()
{
	if (!PhysicSystem::scene)
		return;

	if (actor)
		PhysicSystem::scene->removeActor(*actor);
}