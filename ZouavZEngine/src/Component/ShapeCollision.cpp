#include "Component/ShapeCollision.hpp"
#include "PxActor.h"
#include "PxMaterial.h"
#include "PxShape.h"
#include "PxRigidDynamic.h"
#include "PxRigidStatic.h"
#include "GameObject.hpp"
#include "Component/RigidBody.hpp"
#include "Component/RigidStatic.hpp"
#include "System/Debug.hpp"
#include "System/PhysicUtils.hpp"
#include "imgui.h"

ShapeCollision::ShapeCollision(GameObject* _gameObject, Transform _transform, bool _isTrigger)
	 : Component(_gameObject), transform(_transform), isTrigger(_isTrigger)
{
	
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

	if (rigid)
	{
		physx::PxShape* shape = nullptr;
		rigid->actor->getShapes(&shape, 1);

		if (shape)
			shape->setLocalPose(PxTransformFromTransform(transform));
	}
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

		shape->release();
	}
}

void ShapeCollision::UpdateTransform(Transform _transform)
{
	transform = _transform;

	if (shape)
		shape->setLocalPose(PxTransformFromTransform(transform));
}