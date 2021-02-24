#include "System/Terrain.hpp"
#include "System/ResourcesManager.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Camera.hpp"
#include "Component/Light.hpp"
#include "Maths/Mat4.hpp"
#include <FastNoiseLite.h>
#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include <cstdlib>


Terrain::Terrain()
{
	shader = static_cast<Shader*>(ResourcesManager::GetResource("TerrainShader"));
	AddNoiseLayer();
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
														seed, noiseParams,															
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
														seed, noiseParams,
														minHeight, maxHeight, heightIntensity }, true);
		}
	}
}

void Terrain::Draw(const std::vector<class Light*>& _lights, const class Camera& _camera)
{
	shader->Use();
	shader->SetLight(_lights);
	Mat4 matrixCamera = _camera.GetMatrix();
	shader->SetMatrix("view", matrixCamera.Reversed());
	shader->SetVector3("viewPos", matrixCamera.Accessor(0, 3), matrixCamera.Accessor(1, 3), matrixCamera.Accessor(2, 3));
	shader->SetMatrix("projection", _camera.GetProjetionMatrix());

	for (auto& it : chunks)
	{
		shader->SetMatrix("model", Mat4::CreateTranslationMatrix({ it.second.GetWorldPos().x, 0.f, it.second.GetWorldPos().y}));

		glBindVertexArray(it.second.GetMesh().GetID());
		glDrawElements(GL_TRIANGLES, it.second.GetMesh().GetNbElements(), GL_UNSIGNED_INT, 0);
	}
}

void Terrain::DisplayOptionWindow()
{
	if (ImGui::Begin("Procedural Generation", nullptr, ImGuiWindowFlags_NoNavInputs))
	{
		ImGui::Checkbox("Always Actualize", &alwaysActualize);
		bool actualized = false;
		actualized |= ImGui::SliderInt("Chunk Count", &chunkCount, 1, 16);
		actualized |= ImGui::SliderInt("Chunk Size", &chunkSize, 1, 512);
		actualized |= ImGui::SliderInt("Chunk Vertex Count (LOD)", &chunkVertexCount, 2, 64);

		actualized |= ImGui::SliderInt("Seed", &seed, 0, 214748364);

		if (ImGui::Button("<"))
			noiseID = (noiseID - 1) % (int)noiseParams.size();
		ImGui::SameLine();
		ImGui::Text("%d/%d", noiseID + 1, noiseParams.size());
		ImGui::SameLine();
		if (ImGui::Button(">"))
			noiseID = (noiseID + 1) % (int)noiseParams.size();

		actualized |= ImGui::Combo("Noise Type", (int*)&noiseParams[noiseID].noiseType, "Perlin\0OpenSimplex2\0OpenSimplex2S\0Cellular\0ValueCubic\0");
		actualized |= ImGui::SliderFloat("Frequency", &noiseParams[noiseID].frequency, 0.0f, 2.0f);

		actualized |= ImGui::Combo("Fractal Type", (int*)&noiseParams[noiseID].fractalType, "None\0FBm\0Ridged\0PingPong\0");
		if (noiseParams[noiseID].fractalType != 0)
		{
			actualized |= ImGui::SliderInt("Octaves", &noiseParams[noiseID].octaves, 1, 8);
			actualized |= ImGui::SliderFloat("Lacunarity", &noiseParams[noiseID].lacunarity, 0.0f, 8.0f);
			actualized |= ImGui::SliderFloat("Gain", &noiseParams[noiseID].gain, 0.0f, 8.0f);
			actualized |= ImGui::SliderFloat("Weighted Strength", &noiseParams[noiseID].weightedStrength, 0.0f, 16.0f);
			if (noiseParams[noiseID].fractalType == 3)
				actualized |= ImGui::SliderFloat("PingPong Stength", &noiseParams[noiseID].pingPongStength, 0.0f, 16.0f);
		}

		if (ImGui::Button("Add Layer"))
		{
			actualized = true;
			AddNoiseLayer();
		}
		ImGui::SameLine();
		if (ImGui::Button("Remove Layer"))
		{
			actualized = true;
			DeleteCurrentNoiseLayer();
		}

		actualized |= ImGui::SliderFloat("Minimum Height", &minHeight, -100, maxHeight);
		actualized |= ImGui::SliderFloat("Maximum Height", &maxHeight, minHeight, 100);
		actualized |= ImGui::SliderFloat("Height Intensity", &heightIntensity, 1, 200);

		if (alwaysActualize && actualized || ImGui::Button("Actualize"))
			Actualise();

	}
	ImGui::End();
}

void Terrain::AddNoiseLayer()
{
	if (noiseParams.size() > MAX_NOISE_COUNT)
		return;
	noiseParams.emplace_back(NoiseParam{});
	noiseID = noiseParams.size() - 1;
}

void Terrain::DeleteCurrentNoiseLayer()
{
	if (noiseParams.size() <= 1)
		return;
	noiseParams.erase(noiseParams.begin() + noiseID);
	noiseID = (noiseID - 1) % (int)noiseParams.size();
}

float Chunk::CalculateHeigt(ChunkCreateArg _cca, float _x, float _z)
{
	FastNoiseLite noise;

	int i = 0;

	float height = 0;

	float sampleX = _x / ((float)_cca.vertexCount - 1) * _cca.size + _cca.pos.x * _cca.size;
	float sampleZ = _z / ((float)_cca.vertexCount - 1) * _cca.size + _cca.pos.y * _cca.size;

	for (const NoiseParam& noiseparam : _cca.noiseParams)
	{
		noise.SetSeed(_cca.seed + i++);
		noise.SetNoiseType(noiseparam.noiseType);
		noise.SetFrequency(noiseparam.frequency);
		noise.SetFractalType(noiseparam.fractalType);
		noise.SetFractalOctaves(noiseparam.octaves);
		noise.SetFractalLacunarity(noiseparam.lacunarity);
		noise.SetFractalGain(noiseparam.gain);
		noise.SetFractalWeightedStrength(noiseparam.weightedStrength);
		noise.SetFractalPingPongStrength(noiseparam.pingPongStength);

		height += noise.GetNoise(sampleX, sampleZ);
	}

	return height;
}

void Chunk::Generate(ChunkCreateArg _cca, bool _reGenerate)
{
	pos = _cca.pos;
	size = _cca.size;

	std::vector<Vertex> vertices;
	vertices.reserve((size_t)_cca.vertexCount * (size_t)_cca.vertexCount);
	std::vector<int> indices;
	indices.reserve(6 * ((size_t)_cca.vertexCount - 1) * ((size_t)_cca.vertexCount - 1));

	for (float z = 0.0f; z < _cca.vertexCount; z++)
	{
		for (float x = 0.0f; x < _cca.vertexCount; x++)
		{
			float height = std::clamp(CalculateHeigt(_cca, x, z) * _cca.heightIntensity, _cca.minHeight, _cca.maxHeight);

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