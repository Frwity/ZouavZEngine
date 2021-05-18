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

ShapeCollision::ShapeCollision(GameObject* _gameObject, Transform _transform, bool _isTrigger)
	 : Component(_gameObject), transform(_transform), isTrigger(_isTrigger)
{
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);
	materialShader = Material(*ResourcesManager::GetResource<Shader>("GizmosShader"), *ResourcesManager::GetResource<Texture>("Error"), {1.0f, 0.0f, 0.0f, 1.0f});

	if (!_gameObject->IsActive())
		InternalDehactivate();
}


ShapeCollision::ShapeCollision(const ShapeCollision& _other)
	: Component(_other)
{
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f); // TODO physxsystem material manager
	materialShader = Material(*ResourcesManager::GetResource<Shader>("GizmosShader"), *ResourcesManager::GetResource<Texture>("Error"), {1.0f, 0.0f, 0.0f, 1.0f});
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

	if (rigid)
	{
		int i = 0;
		shapeActor = (physx::PxShape**)malloc(sizeof(physx::PxShape*) * rigid->actor->getNbShapes());
		int j = rigid->actor->getShapes(shapeActor, rigid->actor->getNbShapes());

  		while(i++ < j - 1)
		{
			if (shapeActor[i] == shape)
				shape->setLocalPose(PxTransformFromTransform(transform));
		}

		free(shapeActor);
	}
}

void ShapeCollision::AttachToRigidComponent()
{
	if (shape)
	{
		Rigid* rigid = GetGameObject().GetComponent<Rigid>();

		if (rigid)
		{
			//ZASSERT(shape->getGeometryType() == physx::PxGeometryType::ePLANE && rigid->actor->is<physx::PxRigidDynamic>(), "Plane must be created with a RigidStatic");
			shape = physx::PxRigidActorExt::createExclusiveShape(*rigid->actor, *geometry, *material);
			if (shape)
			{
				shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
				shape->setLocalPose(PxTransformFromTransform(transform));
			}
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

void ShapeCollision::UpdateTransform()
{
	if (shape)
		shape->setLocalPose(PxTransformFromTransform(transform));
}

void ShapeCollision::DrawGizmos(const Camera& _camera)
{
	
}