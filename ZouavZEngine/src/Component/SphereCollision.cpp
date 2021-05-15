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

SphereCollision::SphereCollision(GameObject* _gameObject, float _radius, bool _isTrigger, Transform _transform)
	: ShapeCollision(_gameObject, _transform, _isTrigger), radius(_radius)
{
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);
	
	shape = PhysicSystem::physics->createShape(PxSphereGeometry(_radius), *material);
	//shape->setLocalPose(PxTransformFromTransform(transform));
	//attach shape to rigidbody or rigidstatic if exist
	AttachToRigidComponent();

	sphereMesh = *ResourcesManager::GetResource<Mesh>("Sphere");
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

void SphereCollision::DrawGizmos(const Camera& _camera)
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

	Mat4 mat = Mat4FromPxMat44(m) * Mat4::CreateScaleMatrix(Vec3(radius, radius, radius));

	materialShader.shader->SetMatrix("matrix", mat);
	materialShader.shader->SetMatrix("view", _camera.GetMatrix().Reversed());
	materialShader.shader->SetMatrix("projection", _camera.GetProjetionMatrix());
	materialShader.shader->SetVector4("color", materialShader.color);

	glBindVertexArray(sphereMesh->GetID());
	glDrawElements(GL_TRIANGLES, sphereMesh->GetNbElements(), GL_UNSIGNED_INT, 0);
}