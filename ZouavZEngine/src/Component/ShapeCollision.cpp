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
#include "extensions/PxRigidActorExt.h"

ShapeCollision::ShapeCollision(GameObject* _gameObject, Transform _transform, bool _isTrigger, std::string _name)
	 : Component(_gameObject, _name), transform(_transform), isTrigger(_isTrigger)
{
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);
	gizmoShader = *ResourcesManager::GetResource<Shader>("GizmosShader");;

	if (!_gameObject->IsActive())
		InternalDeactivate();
}

ShapeCollision::~ShapeCollision()
{
	
}

void ShapeCollision::releasePhysXComponent()
{

}

void ShapeCollision::SetTrigger(bool _isTrigger)
{
	isTrigger = _isTrigger;
	UpdateIsTrigger();
}

void ShapeCollision::UpdateIsTrigger()
{
	if (isTrigger)
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
	}
	else
	{
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
	}
}

void ShapeCollision::Editor()
{
	ImGui::Text("Local Position :");
	ImGui::SameLine(); 
	
	if (ImGui::InputFloat3("##positionx", &transform.localPosition.x))
		UpdateShapeTransform();

	static Vec3 localEulerAngles;
	localEulerAngles = transform.localRotation.ToEuler();

	ImGui::Text("Local Rotation :");
	ImGui::SameLine();
	
	if (ImGui::InputFloat3("##rotation", &localEulerAngles.x))
	{
		transform.localRotation = Quaternion(localEulerAngles);
		UpdateShapeTransform();
	}
}

void ShapeCollision::UpdateShapeTransform()
{
	Rigid* rigid = gameObject->GetComponent<Rigid>();
	physx::PxShape** shapeActor = nullptr;
	
	rigid->actor->getGlobalPose();
	
	//transform.UpdateWorldPos(Vec3FromPxVec3(rigid->actor->getGlobalPose().p), QuaternionFromPxQuat(rigid->actor->getGlobalPose().q), {1.0f,1.0f,1.0f});

	if (rigid)
	{
		int i = 0;
		shapeActor = (physx::PxShape**)malloc(sizeof(physx::PxShape*) * rigid->actor->getNbShapes());
		int j = rigid->actor->getShapes(shapeActor, rigid->actor->getNbShapes());

  		while(i++ < j - 1)
		{
			if (shapeActor[i] == shape)
				shape->setLocalPose(PxTransformFromTransformLocal(transform));
		}

		free(shapeActor);
	}
}

void ShapeCollision::AttachToRigidComponent()
{
	Rigid* rigid = GetGameObject().GetComponent<Rigid>();

	if (rigid)
	{
		//ZASSERT(shape->getGeometryType() == physx::PxGeometryType::ePLANE && rigid->actor->is<physx::PxRigidDynamic>(), "Plane must be created with a RigidStatic");
		shape = physx::PxRigidActorExt::createExclusiveShape(*rigid->actor, *geometry, *material);
		if (shape)
		{
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
			shape->setLocalPose(PxTransformFromTransformLocal(transform));
		}
	}
}

void ShapeCollision::Activate()
{
	Component::Activate();
	if (shape)
		UpdateIsTrigger();
}

void ShapeCollision::Deactivate()
{
	Component::Deactivate();
	if (shape)
	{
		if (IsTrigger())
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
		else
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
	}
}

void ShapeCollision::InternalActivate()
{
	if (isActive && shape)
		UpdateIsTrigger();
}

void ShapeCollision::InternalDeactivate()
{
	if (isActive && shape)
	{
		if (IsTrigger())
			shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
		else
			shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
	}
}

void ShapeCollision::EditPosition(const Vec3& _newPos)
{
	transform.localPosition = _newPos;
	UpdateShapeTransform();
}

void ShapeCollision::EditRotation(const Vec3& _newRot)
{
	transform.localRotation = _newRot;
	UpdateShapeTransform();
}

void ShapeCollision::UpdateTransform() // never called
{
	if (shape)
		shape->setLocalPose(PxTransformFromTransformLocal(transform));
}

void ShapeCollision::DrawGizmos(const Camera& _camera)
{

}
