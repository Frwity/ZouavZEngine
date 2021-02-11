#include "System/Terrain.hpp"
#include "System/ResourcesManager.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Camera.hpp"
#include "Component/Light.hpp"
#include "Maths/Mat4.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>
#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>

void Terrain::Generate()
{
	std::vector<Vertex> vertices;
	vertices.reserve(vertexCount * vertexCount);
	std::vector<int> indices;
	indices.reserve(6 * (vertexCount - 1) * (vertexCount - 1));

	float wrap = size / 32;

	for (float i = 0.0f; i < vertexCount; i++)
	{
		for (float j = 0.0f; j < vertexCount; j++)
		{
			float height = stb_perlin_noise3(j / ((float)vertexCount - 1) * wrap, 0, i / ((float)vertexCount - 1) * wrap, 0, 0, 0) * 30 + 10;

			vertices.push_back(Vertex{	Vec3(j / ((float)vertexCount - 1) * size, height, i / ((float)vertexCount - 1) * size),
										Vec3(0.0f, 1.0f, 0.0f),
										Vec2(j / ((float)vertexCount - 1), i / ((float)vertexCount - 1)) });
		}
	}

	int pointer = 0;

	for (int gz = 0; gz < vertexCount - 1; gz++)
	{
		for (int gx = 0; gx < vertexCount - 1; gx++)
		{
			int topLeft = (gz * vertexCount) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * vertexCount) + gx;
			int bottomRight = bottomLeft + 1;
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}

	mesh.InitMesh(vertices.data(), vertices.size(), indices.data(), indices.size());

	shader = static_cast<Shader*>(ResourcesManager::GetResource("TerrainShader"));
}

void Terrain::Draw(const std::vector<Light*>& lights)
{
	shader->Use();
	shader->SetLight(lights);
	Mat4 matrixCamera = Camera::GetMainCamera().GetMatrix();

	shader->SetMatrix("view", matrixCamera.Reverse());
	shader->SetMatrix("projection", Camera::GetMainCamera().GetProjetionMatrix());
	shader->SetMatrix("model", Mat4::Identity());
	shader->SetVector3("viewPos", matrixCamera.Accessor(0, 3), matrixCamera.Accessor(1, 3), matrixCamera.Accessor(2, 3));

	glBindVertexArray(mesh.GetID());
	glDrawElements(GL_TRIANGLES, mesh.GetNbElements(), GL_UNSIGNED_INT, 0);
}