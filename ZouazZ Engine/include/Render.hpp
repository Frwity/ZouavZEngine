#pragma once

class Render
{
private:
	int width, height;
public:
	struct GLFWwindow* window;

	Render(int _width, int _height);
	void Draw();
	~Render() = default;

	//Set int, float, ...

	void Destroy();
};