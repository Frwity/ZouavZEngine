#pragma once
#include "Rendering/Render.hpp"
#include "Rendering/Camera.hpp"
#include "Maths/Mat4.hpp"
#include "Scene.hpp"
#include "System/Editor.hpp"
#include "System/SoundManager.hpp"

enum class ENGINE_STATE
{
	EDITING,
	PLAYING
};


class Engine
{
private:
	Render render;
	ENGINE_STATE state{ ENGINE_STATE::EDITING };
	SceneCamera sceneCamera{Vec2(0, 0), 100, 100 };
	Scene scene;
	Editor editor;

public:

	Engine();
	~Engine();

	void Load();
	void Update();
};