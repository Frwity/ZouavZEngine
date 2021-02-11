#include "GameObject.hpp"
#include "Component/Transform.hpp"
#include "Component/MeshRenderer.hpp"
#include "Rendering/Camera.hpp"
#include "Component/Light.hpp"
#include "Maths/Mat4.hpp"
#include "Scene.hpp"

Scene* Scene::currentScene = nullptr;

Scene::Scene()
{
	currentScene = this;
	world.SetParent(nullptr);
}

void Scene::Draw() const
{
	for(GameObject* gameObject : world.GetChildren()) //TODO test call time
	{
		if (gameObject->GetComponent<MeshRenderer>())
		{
			gameObject->GetComponent<MeshRenderer>()->shader->SetLight(lights);
			gameObject->GetComponent<MeshRenderer>()->Draw(Mat4::Identity(), Camera::GetMainCamera());
		}
		for (GameObject* child : gameObject->GetChildren())
			DrawChild(child, Mat4::CreateTRSMatrix(gameObject->position, gameObject->rotation, gameObject->scale));
	}
}

void Scene::DrawChild(GameObject* _parent, const Mat4& _heritedMatrix) const
{
	if (_parent->GetComponent<MeshRenderer>())
	{
		_parent->GetComponent<MeshRenderer>()->shader->SetLight(lights);
		_parent->GetComponent<MeshRenderer>()->Draw(_heritedMatrix, Camera::GetMainCamera());
	}

	for (GameObject* child : _parent->GetChildren())
		DrawChild(child, _heritedMatrix * Mat4::CreateTRSMatrix(_parent->position, _parent->rotation, _parent->scale));
}
