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
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Component/RigidBody.hpp"
#include "imgui.h"
#include <memory>

using namespace physx;

BoxCollision::BoxCollision(GameObject* _gameObject, Vec3 _halfExtends, bool _isTrigger, Transform _transform)
	: ShapeCollision(_gameObject, _transform, _isTrigger), halfExtends(_halfExtends)
{
	shape = PhysicSystem::physics->createShape(PxBoxGeometry(PxVec3FromVec3(_halfExtends)), *material);

	AttachToRigidComponent();
	cube = *ResourcesManager::GetResource<Mesh>("Default");

	UpdateExtends();
}

BoxCollision::BoxCollision(const BoxCollision& _other)
	: ShapeCollision(_other), halfExtends{ _other.halfExtends }
{
	shape = PhysicSystem::physics->createShape(PxBoxGeometry(PxVec3FromVec3(_other.halfExtends)), *material);

	AttachToRigidComponent();
	cube = *ResourcesManager::GetResource<Mesh>("Default");
}

BoxCollision::~BoxCollision()
{
	 
}

void BoxCollision::Editor()
{
	ShapeCollision::Editor();

	if (ImGui::DragFloat3("Half Extends : ", &halfExtends.x, 0.1f, 0.1f, 100.0f))
		UpdateExtends();

	ImGui::Checkbox("isTrigger", &isTrigger);
}

void BoxCollision::UpdateExtends()
{
	Rigid* rigid = gameObject->GetComponent<Rigid>();

	if (rigid)
		rigid->actor->detachShape(*shape);

	shape = PhysicSystem::physics->createShape(PxBoxGeometry(PxVec3FromVec3(halfExtends) / 2.0f), *material);

	AttachToRigidComponent();
}

void BoxCollision::DrawGizmos(const Camera& _camera)
{
	if (shape == nullptr)
		return;

	materialShader.shader->Use();

	Rigid* rigid = gameObject->GetComponent<Rigid>();

	physx::PxMat44 m;

	if (rigid)
		m = rigid->actor->getGlobalPose();
	else
		m = physx::PxMat44(shape->getLocalPose());

	Mat4 mat = Mat4FromPxMat44(m) * Mat4::CreateScaleMatrix(Vec3(halfExtends.x, halfExtends.y, halfExtends.z));

	materialShader.shader->SetMatrix("matrix", mat);
	materialShader.shader->SetVector4("color", materialShader.color);

	glBindVertexArray(cube->GetID());
	glDrawElements(GL_LINE_LOOP, cube->GetNbElements(), GL_UNSIGNED_INT, 0);
}