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
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace physx;

CapsuleCollision::CapsuleCollision(GameObject* _gameObject, float _radius, float _halfHeight, bool _isTrigger, Transform _transform)
	: ShapeCollision(_gameObject, _transform, _isTrigger), radius(_radius), halfHeight(_halfHeight)
{
	shape = PhysicSystem::physics->createShape(PxCapsuleGeometry(radius, halfHeight), *material);

	AttachToRigidComponent();

	gizmoMesh = *ResourcesManager::GetResource<Mesh>("Capsule");
}

CapsuleCollision::CapsuleCollision(const CapsuleCollision& _other)
	: ShapeCollision(_other), radius(_other.radius), halfHeight(_other.halfHeight)
{
	shape = PhysicSystem::physics->createShape(PxCapsuleGeometry(radius, halfHeight), *material);

	AttachToRigidComponent();

	gizmoMesh = *ResourcesManager::GetResource<Mesh>("Capsule");
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

	if (rigid)
		rigid->actor->detachShape(*shape);

	shape = PhysicSystem::physics->createShape(PxCapsuleGeometry(radius, halfHeight), *material);

	AttachToRigidComponent();
}

void CapsuleCollision::DrawGizmos(const Camera& _camera, const Mat4& _modelMatrix)
{
	ShapeCollision::DrawGizmos(_camera, Mat4::CreateScaleMatrix(Vec3(radius, halfHeight, radius) * gameObject->localScale));
}