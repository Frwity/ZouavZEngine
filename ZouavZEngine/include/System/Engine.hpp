#pragma once
#include "Rendering/Render.hpp"
#include "Rendering/Camera.hpp"
#include "Maths/Mat4.hpp"
#include "Scene.hpp"
#include "System/Editor.hpp"
#include "System/SoundManager.hpp"

class Engine
{
private:
	Editor editor;
	Render render;
	Scene scene;
	SceneCamera sceneCamera{100, 100 };

public:

	Engine();
	~Engine();

	void Load();
	void Update();
};