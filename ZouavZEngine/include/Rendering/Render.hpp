#pragma once

class Render
{
private:
public:
	struct GLFWwindow* window{ nullptr };
	int width{ 0 }, height{ 0 };
	
	Render() {};
	Render(int _width, int _height);
	void Init(int _width, int _height);
	void Update();
	void Draw(const class Mesh& mesh);
	bool Stop();
	void Clear();
	~Render() = default;

	void Destroy();
};