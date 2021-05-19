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
	geometry = new PxSphereGeometry(_radius);
	shape = PhysicSystem::physics->createShape(*geometry, *material);

	//attach shape to a Rigid component if exist
	AttachToRigidComponent();

	sphereMesh = *ResourcesManager::GetResource<Mesh>("Sphere");
	UpdateScale();
}

SphereCollision::SphereCollision(const SphereCollision& _other)
	: ShapeCollision(_other), radius{_other.radius}
{
	shape = PhysicSystem::physics->createShape(PxSphereGeometry(_other.radius), *material);
	
	AttachToRigidComponent();

	sphereMesh = *ResourcesManager::GetResource<Mesh>("Sphere");

} 

SphereCollision::~SphereCollision()
{

}

void SphereCollision::Editor()
{
	ShapeCollision::Editor();

	if (ImGui::SliderFloat("Radius : ", &radius, 0.1f, 100.0f))
		UpdateScale();

	ImGui::Checkbox("isTrigger", &isTrigger);
}

void SphereCollision::UpdateScale()
{
	Rigid* rigid = gameObject->GetComponent<Rigid>();

	if (rigid)
		rigid->actor->detachShape(*shape);

	//TODO world scale
	geometry = new PxSphereGeometry(radius);

	AttachToRigidComponent();
	shape->userData = this;
}

void SphereCollision::DrawGizmos(const Camera& _camera)
{
	if (shape == nullptr)
		return;

	materialShader.shader->Use();

	Mat4 trsLocal = Mat4::CreateTRSMatrix(transform.localPosition, transform.localRotation, Vec3(radius / 1.5f, radius /1.5f, radius / 1.5f));
	Mat4 trsGlobal = Mat4::CreateTRSMatrix(gameObject->WorldPosition(), gameObject->WorldRotation(), gameObject->WorldScale());

	Mat4 mat = trsGlobal * trsLocal;

	materialShader.shader->SetMatrix("matrix", mat);
	materialShader.shader->SetMatrix("view", _camera.GetMatrix().Reversed());
	materialShader.shader->SetMatrix("projection", _camera.GetProjetionMatrix());
	materialShader.shader->SetVector4("color", materialShader.color);

	glBindVertexArray(sphereMesh->GetID());
	glDrawElements(GL_TRIANGLES, sphereMesh->GetNbElements(), GL_UNSIGNED_INT, 0);
}