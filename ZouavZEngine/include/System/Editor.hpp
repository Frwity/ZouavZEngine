#pragma once
#include <string>

class Editor
{
private:
	double lastCursorPosX = 0;
	double lastCursorPosY = 0;
public:
	bool isKeyboardEnable;
	Editor();
	void NewFrame();
	
	//TODO discuss method name (DisplayXXX or XXX)
	void DisplayMainWindow();
	void DisplayMenuBar();
	void DisplaySceneWindow(const class Render& _render, class Framebuffer& _framebuffer);
	void DisplayInspector();
	void DisplayConsoleWindow();
	void DisplayGameWindow(const class Render& _render, class Framebuffer& _framebuffer);
	void FileMenu();
	void Update();
	void DisplayGameWindow();
	void DisplayHierarchy();
};