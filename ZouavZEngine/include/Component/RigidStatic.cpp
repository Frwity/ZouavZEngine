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
	: Rigid(_gameObject)
{
	if (_gameObject == nullptr)
		return;

	PxTransform t(PxVec3FromVec3(GetGameObject().WorldPosition()), PxQuatFromQuaternion(GetGameObject().WorldRotation()));

	actor = PhysicSystem::physics->createRigidStatic(t);

	actor->userData = &GetGameObject();

	AttachShape();

	PhysicSystem::scene->addActor(*actor);
}

RigidStatic::~RigidStatic()
{
	
}

void RigidStatic::Editor()
{

}

template <class Archive>
static void RigidStatic::load_and_construct(Archive& _ar, cereal::construct<RigidStatic>& _construct)
{
	_construct(GameObject::currentLoadedGameObject);
}

void RigidStatic::UpdateTransform(Transform transform)
{
	
}