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

BoxCollision::BoxCollision(GameObject* _gameObject, Vec3 _halfExtends, bool _isTrigger, Transform _transform, std::string _name)
	: ShapeCollision(_gameObject, _transform, _isTrigger), halfExtends(_halfExtends)
{
	name = _name;
	geometry = new PxBoxGeometry(PxVec3FromVec3(_halfExtends));
	shape = nullptr;

	gizmoMesh = *ResourcesManager::GetResource<Mesh>("Default");

	UpdateScale();
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

	if (rigid && shape)
		rigid->actor->detachShape(*shape);

	geometry = new PxBoxGeometry(PxVec3FromVec3(halfExtends * gameObject->WorldScale()) / 2.0f);

	AttachToRigidComponent();
	if(shape)
		shape->userData = this;

	UpdateShapeTransform();
}

void BoxCollision::DrawGizmos(const Camera& _camera)
{
	if (shape == nullptr)
		return;

	gizmoShader->Use();
	
	Vec3 worldPosition = Mat4::CreateTRSMatrix(GetGameObject().WorldScale(), GetGameObject().WorldRotation(), {1.0f, 1.0f ,1.0f}) * transform.localPosition;
	Quaternion worldRotation = GetGameObject().WorldRotation() * transform.localRotation;
	Vec3 worldScale = GetGameObject().WorldScale() * halfExtends;

	Mat4 mat = Mat4::CreateTRSMatrix(worldPosition, worldRotation, worldScale);

	gizmoShader->SetMatrix("matrix", mat);
	gizmoShader->SetVector4("color", Vec4(1.0f, 0.0f, 0.0f, 1.0f));

	glBindVertexArray(gizmoMesh->GetID());
	glDrawElements(GL_LINE_LOOP, gizmoMesh->GetNbElements(), GL_UNSIGNED_INT, 0);
}