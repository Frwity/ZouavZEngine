#pragma once
#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"

class Terrain
{
private:
	int vertexCount = 64;
	int size = 128;

	Mesh mesh{};
	Shader* shader{};
public:
	Terrain() = default;

	void Generate();

	void Draw(const std::vector<class Light*>& lights);
};