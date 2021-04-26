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

static int i = 0;


RigidBody::RigidBody(GameObject* _gameObject)
	: Component(_gameObject)
{
	PxTransform t(PxVec3FromVec3(GetGameObject().WorldPosition()), PxQuatFromQuaternion(GetGameObject().WorldRotation()));

	actor = PhysicSystem::physics->createRigidDynamic(t);

	actor->userData = this;

	if (i++ == 0)
		actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	AttachShape();

	PhysicSystem::scene->addActor(*actor);
}

RigidBody::~RigidBody()
{
	
}

void RigidBody::SetLinearVelocity(const class Vec3& v)
{
	actor->setLinearVelocity(PxVec3FromVec3(v));
}

Vec3 RigidBody::GetLinearVelocity()
{
	return Vec3FromPxVec3(actor->getLinearVelocity());
}

void RigidBody::DisableGravity()
{
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
}

void RigidBody::EnableGravity()
{
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
}

void RigidBody::AttachShape()
{
	//TODO use getComponents
	ShapeCollision* collision = GetGameObject().GetComponent<ShapeCollision>();

	if (collision && !collision->isAttach)
	{
		collision->isAttach = true;
		if (!actor->attachShape(*collision->shape))
			Debug::LogError("Attach to shape failed !");
		collision->shape->release();
	}
}

void RigidBody::OnContact(GameObject* _other)
{
	ScriptComponent* script = GetGameObject().GetComponent<ScriptComponent>();

	if (script)
		script->OnContact(_other);
}

void RigidBody::OnTrigger(GameObject* _other)
{
	ScriptComponent* script = GetGameObject().GetComponent<ScriptComponent>();

	if (script)
		script->OnTrigger(_other);
}

void RigidBody::Editor()
{
}

template <class Archive>
static void RigidBody::load_and_construct(Archive& _ar, cereal::construct<RigidBody>& _construct)
{
	_construct(GameObject::currentLoadedGameObject);
}