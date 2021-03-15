#pragma once
#include "Rendering/Render.hpp"
#include "Maths/Mat4.hpp"
#include "Scene.hpp"
#include "System/Editor.hpp"
#include "System/SoundManager.hpp"

class Engine
{
	friend class Editor;
private:
	Editor editor;
	Render render;
	Scene scene;

public:

	Engine();
	~Engine();

	void Load();
	void Update();
};