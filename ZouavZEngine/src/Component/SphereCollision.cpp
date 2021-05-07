#include "Component/SphereCollision.hpp"
#include "GameObject.hpp"
#include "PxRigidBody.h"
#include "PxRigidStatic.h"
#include "PxShape.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "extensions/PxSimpleFactory.h"
#include "System/PhysicUtils.hpp"
#include "imgui.h"

using namespace physx;

SphereCollision::SphereCollision(GameObject* _gameObject, float _radius, bool _isTrigger, Transform _transform)
	: ShapeCollision(_gameObject, _transform, _isTrigger), radius(_radius)
{
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);
	
	shape = PhysicSystem::physics->createShape(PxSphereGeometry(_radius), *material);
	shape->setLocalPose(PxTransformFromTransform(transform));
	//attach shape to rigidbody or rigidstatic if exist
	AttachToRigidComponent();
}

SphereCollision::~SphereCollision()
{

}

void SphereCollision::Editor()
{
	ShapeCollision::Editor();

	if (ImGui::SliderFloat("Radius : ", &radius, 0.0f, 100.0f))
		UpdateRadius(radius);

	ImGui::Checkbox("isTrigger", &isTrigger);
}

void SphereCollision::UpdateRadius(float _radius)
{
	Rigid* rigid = gameObject->GetComponent<Rigid>();

	if (!rigid)
		return;

	rigid->actor->detachShape(*shape);
	shape->release();

	radius = _radius;
	shape = PhysicSystem::physics->createShape(PxSphereGeometry(radius), *material);

	AttachToRigidComponent();
}