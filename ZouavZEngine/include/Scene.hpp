#pragma once
#include "GameObject.hpp"
#include "System/Terrain.hpp"
#include <vector>

class Scene
{
private:
	static Scene* currentScene;

	Terrain terrain;
	GameObject world{ "World" };

	void DrawChild(GameObject* _parent, const Mat4& _heritedMatrix, const class Camera& _camera) const;
	
	std::vector<class Light*> lights;

public:

	Scene();
	~Scene();

	static Scene* GetCurrentScene() { return currentScene; }
	GameObject& GetWorld() { return world; }

	static void NewScene(const std::string& _sceneName);

	void Load(bool _changingScene = true);
	void Load(const std::string& _path, bool _changingScene = true);
	void Save();

	void Update();
	void Draw(const class Camera& _camera) const;
	void SimulatePhyics() const;					//Simulate phyics and update transform of affected gameObjects

	void AddLight(class Light* _newLight);
	void DeleteLight();
	const std::vector<class Light*>& GetLights() { return lights; }

	void DisplayTerrainOptionWindow();
};
