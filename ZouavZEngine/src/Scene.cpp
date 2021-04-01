#include "GameObject.hpp"
#include "Component/Transform.hpp"
#include "Component/MeshRenderer.hpp"
#include "Rendering/Camera.hpp"
#include "Component/Light.hpp"
#include "Maths/Mat4.hpp"
#include "Scene.hpp"

#include <fstream>
#include "cereal/archives/json.hpp"
#include <iostream>

Scene* Scene::currentScene = nullptr;

Scene::Scene()
{
	currentScene = this;
	world.SetParent(nullptr);
}

Scene::~Scene()
{
	if (currentScene == this)
		currentScene = nullptr;
}

void Scene::Load()
{
	std::ifstream saveFile;
	saveFile.open("save.json", std::ios::binary);
	{
		cereal::JSONInputArchive iarchive(saveFile);

		world.load(iarchive);
	}
	saveFile.close();
}

void Scene::Save()
{
	std::ofstream saveFile;
	saveFile.open("save.json", std::ios::binary);
	{
		cereal::JSONOutputArchive oArchive(saveFile);

		world.save(oArchive);
	}
	saveFile.close();
}

void Scene::Draw(const Camera& _camera) const
{
	for(GameObject* gameObject : world.GetChildren()) //TODO test call time
	{
		if (gameObject->GetComponent<MeshRenderer>())
		{
			gameObject->GetComponent<MeshRenderer>()->shader->SetLight(lights);
			gameObject->GetComponent<MeshRenderer>()->Draw(Mat4::identity, _camera);
		}
		for (GameObject* child : gameObject->GetChildren())
			DrawChild(child, Mat4::CreateTRSMatrix(gameObject->WorldPosition(), gameObject->WorldRotation(), gameObject->WorldScale()), _camera);
	}
}

void Scene::DrawChild(GameObject* _parent, const Mat4& _heritedMatrix, const Camera& _camera) const
{
	if (_parent->GetComponent<MeshRenderer>())
	{
		_parent->GetComponent<MeshRenderer>()->shader->SetLight(lights);
		_parent->GetComponent<MeshRenderer>()->Draw(_heritedMatrix, _camera);
	}

	for (GameObject* child : _parent->GetChildren())
		DrawChild(child, Mat4::CreateTRSMatrix(_parent->WorldPosition(), _parent->WorldRotation(), _parent->WorldScale()), _camera);
}

void Scene::AddLight(Light* _newLight)
{
	lights.push_back(_newLight);
}

void Scene::DeleteLight()
{
	for (auto light = lights.begin(); light != lights.end(); light++)
	{
		if ((*light)->toDestroy)
		{
			lights.erase(light);
			return;
		}
	}
}