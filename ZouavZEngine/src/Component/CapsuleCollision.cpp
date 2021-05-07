#include "Component/CapsuleCollision.hpp"
#include "GameObject.hpp"
#include "PxRigidBody.h"
#include "PxRigidStatic.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "extensions/PxSimpleFactory.h"
#include "System/PhysicUtils.hpp"
#include "imgui.h"

using namespace physx;

CapsuleCollision::CapsuleCollision(GameObject* _gameObject, float _radius, float _halfHeight, bool _isTrigger, Transform _transform)
	: ShapeCollision(_gameObject, _transform, _isTrigger), radius(_radius), halfHeight(_halfHeight)
{
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);

	shape = PhysicSystem::physics->createShape(PxCapsuleGeometry(radius, halfHeight), *material);
	shape->setLocalPose(PxTransformFromTransform(transform));

	AttachToRigidComponent();
}

CapsuleCollision::~CapsuleCollision()
{

}

void CapsuleCollision::Editor()
{
	ShapeCollision::Editor();

	if (ImGui::SliderFloat("Radius : ", &radius, 0.0f, 100.0f))
		UpdateCapsule();
	
	if (ImGui::SliderFloat("HalfHeight : ", &halfHeight, 0.0f, 100.0f))
		UpdateCapsule();

	ImGui::Checkbox("isTrigger", &isTrigger);
}

void CapsuleCollision::UpdateCapsule()
{
	Rigid* rigid = gameObject->GetComponent<Rigid>();

	if (!rigid)
		return;

	rigid->actor->detachShape(*shape);
	shape->release();

	shape = PhysicSystem::physics->createShape(PxCapsuleGeometry(radius, halfHeight), *material);

	AttachToRigidComponent();
}