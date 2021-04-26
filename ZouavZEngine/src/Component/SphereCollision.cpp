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
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);
	
	shape = PhysicSystem::physics->createShape(PxSphereGeometry(_radius), *material);

	shape->userData = gameObject;

	//attach shape to rigidbody or rigidstatic if exist
	AttachToRigidComponent();
}

SphereCollision::~SphereCollision()
{

}

void SphereCollision::Editor()
{
	ImGui::SliderFloat("Radius : ", &radius, 0.0f, 100.0f);
}