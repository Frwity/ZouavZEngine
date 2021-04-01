#pragma once
#include "Rendering/Render.hpp"
#include "Maths/Mat4.hpp"
#include "Scene.hpp"
#include "System/Editor.hpp"
#include "System/SoundManager.hpp"
#include <iostream>

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
	
	void TempLoad();
	void Load();
	void Update();
	void Save();

};