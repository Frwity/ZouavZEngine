#include "Component/RigidStatic.hpp"
#include "GameObject.hpp"
#include "PxRigidStatic.h"
#include "System/PhysicUtils.hpp"
#include "Component/BoxCollision.hpp"
#include "PxScene.h"
#include "System/PhysicSystem.hpp"
#include "imgui.h"

using namespace physx;

RigidStatic::RigidStatic(GameObject* _gameObject)
	: Component(_gameObject)
{
	PxTransform t(PxVec3FromVec3(GetGameObject().WorldPosition()), PxQuatFromQuaternion(GetGameObject().WorldRotation()));

	actor = PhysicSystem::physics->createRigidStatic(t);

	actor->userData = &GetGameObject();

	AttachShape();

	PhysicSystem::scene->addActor(*actor);

	if (!_gameObject->IsActive())
		InternalDehactivate();
}

void RigidStatic::AttachShape()
{
	//TODO use getComponents
	ShapeCollision* collision = GetGameObject().GetComponent<ShapeCollision>();

	if (collision && !collision->isAttach)
	{
		collision->isAttach = true;
		actor->attachShape(*collision->shape);
		collision->shape->release();
	}
}

void RigidStatic::Activate()
{
	Component::Activate();
	actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
}

void RigidStatic::Dehactivate()
{
	Component::Dehactivate();
	actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
}

void RigidStatic::InternalActivate()
{
	if (isActive)
		actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
}

void RigidStatic::InternalDehactivate()
{
	if (isActive)
		actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
}

RigidStatic::~RigidStatic()
{
	//actor->release();
}

void RigidStatic::Editor()
{
}

template <class Archive>
static void RigidStatic::load_and_construct(Archive& _ar, cereal::construct<RigidStatic>& _construct)
{
	_construct(GameObject::currentLoadedGameObject);
	_ar(cereal::base_class<Component>(_construct.ptr()));
}