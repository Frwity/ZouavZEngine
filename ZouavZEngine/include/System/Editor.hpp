#pragma once
#include <string>
#include "imgui.h"

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

	ImGuiStyle* imguiStyle{nullptr};

	EDITOR_STATE state{ EDITOR_STATE::EDITING };

public:
	bool isKeyboardEnable;
	Editor();

	void Init();
	void NewFrame();
	
	const EDITOR_STATE& GetState() const { return state; }

	//TODO discuss method name (DisplayXXX or XXX)
	void DisplayMainWindow();
	void DisplayOptionWindow();
	void DisplayMenuBar();
	void DisplaySceneWindow(const class Render& _render, class Framebuffer& _framebuffer);
	void DisplayInspector();
	void DisplayConsoleWindow();
	void DisplayGameWindow(const class Render& _render, class Framebuffer& _framebuffer);
	void FileMenu();
	void Update();
	void DisplayHierarchy();
	void MoveSelectedGameobject();
};