#include "Component/ShapeCollision.hpp"
#include "PxActor.h"
#include "PxMaterial.h"
#include "PxShape.h"
#include "PxRigidDynamic.h"
#include "PxRigidStatic.h"
#include "GameObject.hpp"
#include "Component/RigidBody.hpp"
#include "Component/RigidStatic.hpp"
#include "System/PhysicSystem.hpp"
#include "System/Debug.hpp"
#include "System/PhysicUtils.hpp"
#include "System/ResourcesManager.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"

ShapeCollision::ShapeCollision(GameObject* _gameObject, Transform _transform, bool _isTrigger)
	 : Component(_gameObject), transform(_transform), isTrigger(_isTrigger)
{
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);
	gizmoShader = *ResourcesManager::GetResource<Shader>("GizmosShader");;

	if (!_gameObject->IsActive())
		InternalDehactivate();
}


ShapeCollision::ShapeCollision(const ShapeCollision& _other)
	: Component(_other)
{
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f); // TODO physxsystem material manager
	gizmoShader = *ResourcesManager::GetResource<Shader>("GizmosShader");;
	isTrigger = _other.isTrigger;
	if (!_other.IsActive())
		InternalDehactivate();
}

ShapeCollision::~ShapeCollision()
{
	
}

void ShapeCollision::releasePhysXComponent()
{

}

void ShapeCollision::UpdateIsTrigger()
{
	shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
}

void ShapeCollision::Editor()
{
	ImGui::Text("Local Position :");
	ImGui::SameLine(); ImGui::InputFloat3("##positionx", &transform.localPosition.x);

	static Vec3 localEulerAngles;
	localEulerAngles = transform.localRotation.ToEuler();

	ImGui::Text("Local Rotation :");
	ImGui::SameLine(); 
	
	if (ImGui::InputFloat3("##rotation", &localEulerAngles.x))
		transform.localRotation = Quaternion(localEulerAngles);

	//ImGui::Text("Local Scale    :");
	//ImGui::SameLine(); ImGui::InputFloat3("##scalex", &transform.localScale.x);

	Rigid* rigid = gameObject->GetComponent<Rigid>();

	/*if (rigid)
	{
		physx::PxShape* shape = nullptr;
		rigid->actor->getShapes(&shape, 1);

		//if (shape)
			//shape->setLocalPose(PxTransformFromTransform(transform));
	}*/
}

void ShapeCollision::AttachToRigidComponent()
{
	if (shape)
	{
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);

		RigidBody* rd = GetGameObject().GetComponent<RigidBody>();
		RigidStatic* rs = GetGameObject().GetComponent<RigidStatic>();

		if (rd)
		{
			ZASSERT(shape->getGeometryType() != physx::PxGeometryType::ePLANE, "Plane must be created with a RigidStatic");
			rd->actor->attachShape(*shape);
		}
		
		if (rs)
		{
			rs->actor->attachShape(*shape);
		}

	}
}

void ShapeCollision::Activate()
{
	Component::Activate();
	shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
}

void ShapeCollision::Dehactivate()
{
	Component::Dehactivate();
	if(shape)
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
}

void ShapeCollision::InternalActivate()
{
	if (isActive && shape)
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
}

void ShapeCollision::InternalDehactivate()
{
	if (isActive && shape)
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
}

void ShapeCollision::UpdateTransform(Transform _transform)
{
	transform = _transform;

	//does not work atm
	//if (shape)
	//	shape->setLocalPose(PxTransformFromTransform(transform));
}

void ShapeCollision::DrawGizmos(const Camera& _camera, const Mat4& _modelMatrix)
{
	if (shape == nullptr)
		return;

	gizmoShader.get()->Use();

	Rigid* rigid = gameObject->GetComponent<Rigid>();

	physx::PxMat44 m;

	if (rigid)
		m = rigid->actor->getGlobalPose();
	else
		m = physx::PxMat44(shape->getLocalPose());

	gizmoShader.get()->SetMatrix("matrix", Mat4FromPxMat44(m) * _modelMatrix);

	gizmoShader.get()->SetVector4("color", { 0.0f, 1.0f, 0.0f, 1.0f });

	glBindVertexArray(gizmoMesh.get()->GetID());
	glDrawElements(GL_LINE_LOOP, gizmoMesh.get()->GetNbElements(), GL_UNSIGNED_INT, 0);
}