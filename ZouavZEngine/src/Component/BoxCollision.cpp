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
	geometry = new PxBoxGeometry(PxVec3FromVec3(_halfExtends));
	shape = PhysicSystem::physics->createShape(*geometry, *material, true);

	AttachToRigidComponent();
	cube = *ResourcesManager::GetResource<Mesh>("Default");

	UpdateScale();
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
		UpdateScale();

	ImGui::Checkbox("isTrigger", &isTrigger);
}

void BoxCollision::UpdateScale()
{
	Rigid* rigid = gameObject->GetComponent<Rigid>();

	if (rigid)
		rigid->actor->detachShape(*shape);

	shape = PhysicSystem::physics->createShape(PxBoxGeometry(PxVec3FromVec3(halfExtends * gameObject->WorldScale()) / 2.0f), *material);

	AttachToRigidComponent();
}

void BoxCollision::DrawGizmos(const Camera& _camera)
{
	if (shape == nullptr)
		return;

	materialShader.shader->Use();
	
	Mat4 trsLocal = Mat4::CreateTRSMatrix(transform.localPosition, transform.localRotation, Vec3(halfExtends.x, halfExtends.y, halfExtends.z));
	Mat4 trsGlobal = Mat4::CreateTRSMatrix(gameObject->WorldPosition(), gameObject->WorldRotation(), gameObject->WorldScale());

	Mat4 mat = trsGlobal * trsLocal;

	materialShader.shader->SetMatrix("matrix", mat);
	materialShader.shader->SetMatrix("view", _camera.GetMatrix().Reversed());
	materialShader.shader->SetMatrix("projection", _camera.GetProjetionMatrix());
	materialShader.shader->SetVector4("color", materialShader.color);

	glBindVertexArray(cube->GetID());
	glDrawElements(GL_LINE_LOOP, cube->GetNbElements(), GL_UNSIGNED_INT, 0);
}