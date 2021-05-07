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
#include "extensions/PxRigidActorExt.h"
#include "System/PhysicUtils.hpp"
#include "imgui.h"

using namespace physx;

BoxCollision::BoxCollision(GameObject* _gameObject, Vec3 _halfExtends, bool _isTrigger, Transform _transform)
	: ShapeCollision(_gameObject, _transform, _isTrigger), halfExtends(_halfExtends)
{
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);
	
	shape = PhysicSystem::physics->createShape(PxBoxGeometry(PxVec3FromVec3(_halfExtends)), *material, true);
	shape->setLocalPose(PxTransformFromTransform(transform));

	AttachToRigidComponent();
}

BoxCollision::~BoxCollision()
{
	
}

void BoxCollision::Editor()
{
	ShapeCollision::Editor();

	if (ImGui::SliderFloat3("Half Extends : ", &halfExtends.x, 0.1f, 100.0f))
		UpdateExtends(halfExtends);

	ImGui::Checkbox("isTrigger", &isTrigger);
}

void BoxCollision::UpdateExtends(const Vec3& v)
{
	halfExtends = v;
	//TODO search for shape attach to the PxActor
	/*Rigid* rigid = gameObject->GetComponent<Rigid>();

	rigid->actor->detachShape(*shape);
	shape->release();
	shape = PhysicSystem::physics->createShape(PxBoxGeometry(PxVec3FromVec3(v)), *material);

	AttachToRigidComponent();*/
}