#pragma once

class Render
{
private:
public:
	struct GLFWwindow* window;
	int width, height;

	Render(int _width, int _height);
	void Draw();
	~Render() = default;

	//Set int, float, ...

	void Destroy();
};