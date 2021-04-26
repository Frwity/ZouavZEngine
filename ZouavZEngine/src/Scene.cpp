#include "GameObject.hpp"
#include "Component/Transform.hpp"
#include "Component/MeshRenderer.hpp"
#include "Rendering/Camera.hpp"
#include "Component/Light.hpp"
#include "Maths/Mat4.hpp"
#include "Scene.hpp"
#include "System/PhysicSystem.hpp"
#include "System/TimeManager.hpp"
#include "PxActor.h"
#include "PxRigidDynamic.h"
#include "PxScene.h"
#include "System/Debug.hpp"
#include "PxSimulationEventCallback.h"
#include "PxRigidStatic.h"
#include "pvd/PxPvd.h"
#include "PhysX/foundation/PxMat44.h"
#include "Component/RigidBody.hpp"

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

void Scene::Load(bool _changingScene)
{
	Load("resources/" + world.name + ".zes", _changingScene);
}

void Scene::Load(const std::string& _path, bool _changingScene)
{
	GameObject::gameObjects.clear();
	world.children.clear();
	PhysicSystem::Destroy();
	PhysicSystem::Init();

	std::ifstream saveFile;
	//if (_changingScene)
	//{
	//	saveFile.open(_path + "r", std::ios::binary);
	//	{
	//		cereal::JSONInputArchive iarchive(saveFile);

	//		ResourcesManager::Clear();
	//		ResourcesManager::load(iarchive);
	//	}
	//	saveFile.close();
	//}
	saveFile.open(_path, std::ios::binary);
	{
		cereal::JSONInputArchive iarchive(saveFile);

		world.load(iarchive);

		terrain = Terrain{};
		terrain.load(iarchive);
	}
	saveFile.close();

	terrain.Generate(GameObject::GetGameObjectByTag("Player"));
	terrain.Update();
}

void Scene::Save()
{
	std::ofstream saveFile;

	saveFile.open(std::string("resources/" + world.name + ".zesr"), std::ios::binary);
	{
		cereal::JSONOutputArchive oArchive(saveFile);
		//ResourcesManager::save(oArchive);
	}
	saveFile.close();

	saveFile.open(std::string("resources/" + world.name + ".zes"), std::ios::binary);
	{
		cereal::JSONOutputArchive oArchive(saveFile);

		world.save(oArchive);

		terrain.save(oArchive);
	}
	saveFile.close();
}

void Scene::Draw(const Camera& _camera) const
{
	terrain.Draw(_camera);

	for(GameObject* gameObject : world.GetChildren())
	{
		if (gameObject->GetComponent<MeshRenderer>())
		{
			gameObject->GetComponent<MeshRenderer>()->material.shader->SetLight(lights);
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
		_parent->GetComponent<MeshRenderer>()->material.shader->SetLight(lights);
		_parent->GetComponent<MeshRenderer>()->Draw(_heritedMatrix, _camera);
	}

	for (GameObject* child : _parent->GetChildren())
		DrawChild(child, Mat4::CreateTRSMatrix(_parent->WorldPosition(), _parent->WorldRotation(), _parent->WorldScale()), _camera);
}

void Scene::Update()
{
	terrain.Update();
	SimulatePhyics();
}

void Scene::SimulatePhyics() const
{
	PhysicSystem::scene->simulate(TimeManager::GetDeltaTime());
	PhysicSystem::scene->fetchResults(true);

	physx::PxU32 nbActiveActor;

	physx::PxActor** activeActors = PhysicSystem::scene->getActiveActors(nbActiveActor);

	for (unsigned int i = 0; i < nbActiveActor; i++)
	{
		RigidBody* rigidbody = static_cast<RigidBody*>(activeActors[i]->userData);

		physx::PxTransform transform = rigidbody->actor->getGlobalPose();
		physx::PxMat44 mat4(transform);

		rigidbody->GetGameObject().localPosition = { transform.p.x, transform.p.y, transform.p.z };
		rigidbody->GetGameObject().localRotation = { transform.q.w,  transform.q.x, transform.q.y, transform.q.z };
	}
}
void Scene::DisplayTerrainOptionWindow()
{
	terrain.DisplayOptionWindow();
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
