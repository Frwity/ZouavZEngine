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
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);
	
	shape = PhysicSystem::physics->createShape(PxBoxGeometry(PxVec3FromVec3(_halfExtends)), *material);
	//shape->setLocalPose(transform);
	AttachToRigidComponent();
}

BoxCollision::~BoxCollision()
{
	
}

void BoxCollision::Editor()
{
	if (ImGui::SliderFloat3("Half Extends : ", &halfExtends.x, 0.1f, 100.0f))
		UpdateExtends(halfExtends);

	ImGui::Checkbox("isTrigger", &isTrigger);
}

void BoxCollision::UpdateExtends(const Vec3& v)
{
	halfExtends = v;
	Rigid* rigid = gameObject->GetComponent<Rigid>();

	rigid->actor->detachShape(*shape);
	shape->release();
	shape = PhysicSystem::physics->createShape(PxBoxGeometry(PxVec3FromVec3(v)), *material);

	AttachToRigidComponent();
}