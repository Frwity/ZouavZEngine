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

Terrain::Terrain()
{
	shader = static_cast<Shader*>(ResourcesManager::GetResource("TerrainShader"));
}

void Terrain::Generate()
{
	chunks.reserve((size_t)chunkCount * (size_t)chunkCount);
	Vec2 pos;
	for (float z = 0; z < chunkCount; z++)
	{
		for (float x = 0; x < chunkCount; x++)
		{
			pos = { x, z };
			chunks.emplace(pos.ToString(), Chunk());
			chunks.at(pos.ToString()).Generate({ pos,	chunkSize, chunkVertexCount, 
														octaves, persistance, lacunarity, 
														minHeight, maxHeight, heightIntensity }, false);
		}
	}
}

void Terrain::Actualise()
{
	Vec2 pos;
	for (float z = 0; z < chunkCount; z++)
	{
		for (float x = 0; x < chunkCount; x++)
		{
			pos = { x, z };
			chunks.at(pos.ToString()).Generate({ pos,	chunkSize, chunkVertexCount,
														octaves, persistance, lacunarity,
														minHeight, maxHeight, heightIntensity }, true);
		}
	}
}

void Terrain::Draw(const std::vector<class Light*>& _lights)
{
	shader->Use();
	shader->SetLight(_lights);
	Mat4 matrixCamera = Camera::GetMainCamera().GetMatrix();
	shader->SetMatrix("view", matrixCamera.Reverse());
	shader->SetVector3("viewPos", matrixCamera.Accessor(0, 3), matrixCamera.Accessor(1, 3), matrixCamera.Accessor(2, 3));
	shader->SetMatrix("projection", Camera::GetMainCamera().GetProjetionMatrix());

	for (auto& it : chunks)
	{
		shader->SetMatrix("model", Mat4::CreateTranslationMatrix({ it.second.GetWorldPos().x, 0.f, it.second.GetWorldPos().y}));

		glBindVertexArray(it.second.GetMesh().GetID());
		glDrawElements(GL_TRIANGLES, it.second.GetMesh().GetNbElements(), GL_UNSIGNED_INT, 0);
	}
}

void Chunk::Generate(ChunkCreateArg _cca, bool _reGenerate)
{
	pos = _cca.pos;
	size = _cca.size;

	std::vector<Vertex> vertices;
	vertices.reserve((size_t)_cca.vertexCount * (size_t)_cca.vertexCount);
	std::vector<int> indices;
	indices.reserve(6 * ((size_t)_cca.vertexCount - 1) * ((size_t)_cca.vertexCount - 1));

	float wrap = (float)size / 32.f;

	for (float z = 0.0f; z < _cca.vertexCount; z++)
	{
		for (float x = 0.0f; x < _cca.vertexCount; x++)
		{
			// calculate height via perlin
			float height = stb_perlin_noise3(x / ((float)_cca.vertexCount - 1) * wrap + _cca.pos.x * wrap, 0, z / ((float)_cca.vertexCount - 1) * wrap + _cca.pos.y * wrap, 0, 0, 0) * _cca.heightIntensity + _cca.minHeight;

			// create vertices
			vertices.push_back(Vertex{ Vec3(x / ((float)_cca.vertexCount - 1) * size, height, z / ((float)_cca.vertexCount - 1) * size),
										Vec3(0.0f, 1.0f, 0.0f),
										Vec2(x / ((float)_cca.vertexCount - 1), z / ((float)_cca.vertexCount - 1)) });
		}
	}

	// indices
	for (int z = 0; z < _cca.vertexCount - 1; z++)
	{
		for (int x = 0; x < _cca.vertexCount - 1; x++)
		{
			int topLeft = (z * _cca.vertexCount) + x;
			int topRight = topLeft + 1;
			int bottomLeft = ((z + 1) * _cca.vertexCount) + x;
			int bottomRight = bottomLeft + 1;
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}

	if (_reGenerate)
		mesh.ChangeSizeAndData(vertices.data(), vertices.size(), indices.data(), indices.size());
	else
		mesh.InitMesh(vertices.data(), vertices.size(), indices.data(), indices.size());
}