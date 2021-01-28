#pragma once
#include "Rendering/Render.hpp"
#include "Rendering/Camera.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Mesh.hpp"
#include "Maths/Matrix4.hpp"
#include "Rendering/Texture.hpp"

class Engine
{
private:
	Render render;
public:
	float deltaTime = 0.0f;
	Camera camera;
	Matrix4 projection;

	//TEMP
	/*Shader shader;
	Mesh mesh;
	Texture texture;*/
	//float lastFrame;

	Engine();
	~Engine();
	void Update();
};