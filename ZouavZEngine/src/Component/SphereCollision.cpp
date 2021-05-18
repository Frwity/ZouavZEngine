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
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Component/RigidBody.hpp"

using namespace physx;

SphereCollision::SphereCollision(GameObject* _gameObject, float _radius, bool _isTrigger, Transform _tranform)
	: ShapeCollision(_gameObject, _tranform, _isTrigger), radius(_radius)
{	
	shape = PhysicSystem::physics->createShape(PxSphereGeometry(_radius), *material);

	//attach shape to rigidbody or rigidstatic if exist
	AttachToRigidComponent();

	gizmoMesh = *ResourcesManager::GetResource<Mesh>("Sphere");
}

SphereCollision::SphereCollision(const SphereCollision& _other)
	: ShapeCollision(_other), radius{_other.radius}
{
	shape = PhysicSystem::physics->createShape(PxSphereGeometry(_other.radius), *material);
	
	AttachToRigidComponent();

	gizmoMesh = *ResourcesManager::GetResource<Mesh>("Sphere");

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

	radius = _radius;
	shape = PhysicSystem::physics->createShape(PxSphereGeometry(radius), *material);

	AttachToRigidComponent();
}

void SphereCollision::DrawGizmos(const Camera& _camera, const Mat4& _modelMatrix)
{
	ShapeCollision::DrawGizmos(_camera, Mat4::CreateScaleMatrix(Vec3(radius, radius, radius) * gameObject->localScale));
}