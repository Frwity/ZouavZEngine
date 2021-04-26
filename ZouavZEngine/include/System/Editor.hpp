#pragma once
#include <string>
#include "imgui.h"
#include "Rendering/Camera.hpp"

enum class EDITOR_STATE
{
	EDITING,
	PLAYING,
	PAUSE
};

class Editor
{
private:
	double lastCursorScenePosX = 0;
	double lastCursorScenePosY = 0;
	double lastCursorGamePosX = 0;
	double lastCursorGamePosY = 0;

	ImGuiStyle* imguiStyle = nullptr;

	EDITOR_STATE state{ EDITOR_STATE::EDITING };

	SceneCamera sceneCamera{ 100, 100 };

	class Clock* editorClock = nullptr;

	class Engine& engine;

public:
	bool isKeyboardEnable;
	Editor(class Engine& _engine);

	void Init();
	void NewFrame();
	
	const EDITOR_STATE& GetState() const { return state; }

	void Display(Render& _render);

	void DisplayMainWindow();
	void DisplayOptionWindow();
	void DisplayMenuBar();
	void DisplaySceneWindow(const class Render& _render, class Framebuffer& _framebuffer);
	void DisplayInspector();
	void DisplayConsoleWindow();
	void DisplayGameWindow(const class Render& _render, class Framebuffer& _framebuffer);
	void DisplayProject();
	void FileMenu();
	void Update();
	void DisplayHierarchy();
	void MoveSelectedGameobject();
};