#pragma once
#include "Rendering/Render.hpp"
#include "Rendering/Camera.hpp"
#include "Maths/Mat4.hpp"
#include "Scene.hpp"

class Engine
{
private:
	Render render;
public:
	Camera camera;
	Scene scene;

	void Load();

	Engine();
	~Engine();
	void Update();
};