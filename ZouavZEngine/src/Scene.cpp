#include "GameObject.hpp"
#include "Component/Transform.hpp"
#include "Component/MeshRenderer.hpp"
#include "Component/FontComponent.hpp"
#include "Component/Light.hpp"
#include "Rendering/Camera.hpp"
#include "Component/Skybox.hpp"
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
#include "Component/ShapeCollision.hpp"

#include <fstream>
#include "cereal/archives/json.hpp"
#include <iostream>
#include <filesystem>

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

bool Scene::NewScene(const std::string& _sceneName, bool _force)
{
	if (!_force && std::filesystem::exists("Project/scenes/" + _sceneName + ".zes"))
		return false;

	GameObject& sceneWorld = GetCurrentScene()->world;
	sceneWorld.name = _sceneName;
	GameObject::gameObjects.clear();
	sceneWorld.children.clear();
	PhysicSystem::scene->release();
	PhysicSystem::InitScene();
	ResourcesManager::Clear();	
	GetCurrentScene()->Save();
}

void Scene::Load(bool _changingScene)
{
	Load("Project/scenes/" + world.name + ".zes", _changingScene);
}

void Scene::Load(const std::string& _path, bool _changingScene)
{
	GameObject::gameObjects.clear();
	world.children.clear();
	PhysicSystem::Destroy();
	PhysicSystem::Init();

	std::ifstream saveFile;
	if (_changingScene)
	{
		saveFile.open(_path + "r", std::ios::binary);
		{
			cereal::JSONInputArchive iarchive(saveFile);

			ResourcesManager::Clear();
			ResourcesManager::load(iarchive);
		}
		saveFile.close();
	}
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

	saveFile.open(std::string("Project/scenes/" + world.name + ".zesr"), std::ios::binary);
	{
		cereal::JSONOutputArchive oArchive(saveFile);
		ResourcesManager::save(oArchive);
	}
	saveFile.close();

	saveFile.open(std::string("Project/scenes/" + world.name + ".zes"), std::ios::binary);
	{
		cereal::JSONOutputArchive oArchive(saveFile);

		world.save(oArchive);

		terrain.save(oArchive);
	}
	saveFile.close();
}

void Scene::UpdateShaderUniform(const Camera& _camera)
{
	for (auto& shader : ResourcesManager::GetResources<Shader>())
	{
		shader.second->Use();

		Mat4 matrixCamera = _camera.GetMatrix();

		shader.second->SetMatrix("view", matrixCamera.Reversed());
		shader.second->SetMatrix("projection", _camera.GetProjetionMatrix());
		shader.second->SetVector3("viewPos", matrixCamera.Accessor(0, 3), matrixCamera.Accessor(1, 3), matrixCamera.Accessor(2, 3));
		shader.second->SetLight(lights);
	}
}

void Scene::Draw(GameObject* _parent, const Camera* _camera) const
{
	if (!_parent->IsActive())
		return;
	if (_parent->GetComponent<MeshRenderer>() && _parent->GetComponent<MeshRenderer>()->IsActive())
		_parent->GetComponent<MeshRenderer>()->Draw(*_camera);
	if (_parent->GetComponent<Skybox>())
		_parent->GetComponent<Skybox>()->Draw(*_camera);

	std::vector<ShapeCollision*> shapes = _parent->GetComponents<ShapeCollision>();

	for (ShapeCollision* shape : shapes)
		shape->DrawGizmos(*_camera);
	
		
	for (GameObject* child : _parent->GetChildren())
		Draw(child, _camera);
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
