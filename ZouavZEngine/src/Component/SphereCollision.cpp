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

SphereCollision::SphereCollision(GameObject* _gameObject, float _radius, bool _isTrigger)
	: ShapeCollision(_gameObject, _isTrigger), radius(_radius)
{	
	shape = PhysicSystem::physics->createShape(PxSphereGeometry(_radius), *material);

	//attach shape to rigidbody or rigidstatic if exist
	AttachToRigidComponent();
}

SphereCollision::SphereCollision(const SphereCollision& _other)
	: ShapeCollision(_other), radius{_other.radius}
{
	shape = PhysicSystem::physics->createShape(PxSphereGeometry(_other.radius), *material);
	AttachToRigidComponent();
}

SphereCollision::~SphereCollision()
{

}

void SphereCollision::Editor()
{
	ImGui::SliderFloat("Radius : ", &radius, 0.0f, 100.0f);
}