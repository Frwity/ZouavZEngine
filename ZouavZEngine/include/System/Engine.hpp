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
	Render render;
public:
	Camera camera;
	Scene scene;
	Editor editor;
	SoundManager soundManager;

	void Load();

	Engine();
	~Engine();
	void Update();
};