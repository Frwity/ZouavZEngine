#pragma once
#include <string>

class Editor
{
private:

public:
	bool isKeyboardEnable;
	Editor();
	void NewFrame();
	
	//TODO discuss method name (Display XXX or XXX)
	void DisplayMainWindow();
	void DisplayMenuBar();
	void DisplaySceneWindow(const class Render& _render, class Framebuffer& _framebuffer);
	void DisplayInspector();
	void DisplayConsoleWindow();
	void FileMenu();
	void Update();
};