#pragma once

class Render
{
private:
public:
	struct GLFWwindow* window;
	int width, height;
	
	Render() {};
	Render(int _width, int _height);
	void Init(int _width, int _height);
	void Update();
	void Draw(const class Mesh& mesh);
	bool Stop();
	void Clear();
	~Render() = default;

	//Set int, float, ...

	void Destroy();
};