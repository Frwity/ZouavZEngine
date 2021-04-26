#include "GameObject.hpp"
#include "System/ResourcesManager.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Camera.hpp"
#include "Component/Light.hpp"
#include "Component/MeshRenderer.hpp"
#include "Maths/Mat4.hpp"
#include <FastNoiseLite.h>
#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "Scene.hpp"

#include "PxRigidStatic.h"
#include "PxScene.h"
#include "PxShape.h"
#include "PxMaterial.h"
#include "System/PhysicUtils.hpp"
#include "System/PhysicSystem.hpp"
#include "foundation/PxTransform.h"
#include "geometry/PxHeightField.h"
#include "geometry/PxHeightFieldFlag.h"
#include "geometry/PxHeightFieldDesc.h"
#include "geometry/PxHeightFieldSample.h"
#include "geometry/PxHeightFieldGeometry.h"
#include "extensions/PxRigidActorExt.h"
#include "System/Terrain.hpp"

using namespace physx;

Terrain::Terrain()
{
	AddColorLayer();
	AddNoiseLayer();
}

void Terrain::Generate(GameObject* _actualizer)
{
	material.emplace_back(PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f));

	if (noiseCount <= 0)
		AddNoiseLayer();
	if (colorCount <= 0)
		AddColorLayer();

	shader = ResourcesManager::GetResource<Shader>("TerrainShader");
	actualizer = _actualizer;
	chunks.reserve(16);
	Vec2 pos;
	for (float z = 4; z < 4; z++)
	{
		for (float x = 0; x < 4; x++)
		{
			pos = { x, z };
			chunks.emplace(pos.ToString(), Chunk{});
			chunks.at(pos.ToString()).Generate({ &material, pos,	chunkSize, chunkVertexCount,
															seed, noiseParams,
															minHeight, maxHeight, heightIntensity }, false);
		}
	}

	isGenerated = true;
}

void Terrain::Actualise()
{
	if (!isGenerated)
		return;
	Vec2 pos;

	std::unordered_map<std::string, Chunk>::iterator it = chunks.begin();

	while (it != chunks.end())
	{
		it->second.Generate({ &material,	it->second.GetPos(), chunkSize, chunkVertexCount,
											seed, noiseParams,
											minHeight, maxHeight, heightIntensity }, true);
		it++;
	}
}

void Terrain::Update()
{
	if (!actualizer || !isGenerated)
		return;

	Vec2 pos;

	Vec2 actualizerPos(actualizer->WorldPosition().x, actualizer->WorldPosition().z);

	std::unordered_map<std::string, Chunk>::iterator it = chunks.begin();

	// delete far Chunk
	while (it != chunks.end())
	{
		if ((it->second.GetWorldPos() - actualizerPos).GetMagnitude() > chunkDistanceRadius + chunkSize)
		{
			//std::cout << "delete " << it->first << " of " << (it->second.GetWorldPos() - actualizerPos).GetMagnitude() << std::endl;
			it = chunks.erase(it);
		}
		else
			it++;
	}

	int chunkRadius = chunkDistanceRadius / chunkSize;

	Vec2 actualizerChunkPos = actualizerPos / (float)chunkSize;

	// create near Chunk
	for (int x = actualizerChunkPos.x - chunkRadius; x < actualizerChunkPos.x + chunkRadius; ++x)
	{
		for (int y = actualizerChunkPos.y - chunkRadius; y < actualizerChunkPos.y + chunkRadius; ++y)
		{
			pos = { (float)x, (float)y };

			if ((pos * (float)chunkSize - actualizerPos).GetMagnitude() > chunkDistanceRadius + chunkSize)
				break;

			if (chunks.find(pos.ToString()) == chunks.end())
			{
				//std::cout << "create " << pos.ToString() << " of " << std::sqrtf(x * chunkSize * x * chunkSize + y * chunkSize * y * chunkSize) << std::endl;
				chunks.emplace(pos.ToString(), Chunk());
				chunks.at(pos.ToString()).Generate({ &material, pos,	chunkSize, chunkVertexCount,
																seed, noiseParams,
																minHeight, maxHeight, heightIntensity }, false);
			}
		}
	}

	return;
}

void Terrain::Draw(const class Camera& _camera) const
{
	if (!shader || !isGenerated)
		return;

	shader->Use();
	shader->SetLight(Scene::GetCurrentScene()->GetLights());
	Mat4 matrixCamera = _camera.GetMatrix();
	shader->SetMatrix("view", matrixCamera.Reversed());
	shader->SetVector3("viewPos", matrixCamera.Accessor(0, 3), matrixCamera.Accessor(1, 3), matrixCamera.Accessor(2, 3));
	shader->SetMatrix("projection", _camera.GetProjetionMatrix());

	shader->SetInt("colorCount", colorCount);

	shader->SetFloatArray("colorHeight",  colorHeight.data(), colorCount);
	shader->SetFloatArray("colorBlend", colorBlend.data(), colorCount);
	shader->SetFloatArray("colorStrength", colorStrength.data(), colorCount);
	shader->SetFloatArray("textureScale", textureScale.data(), colorCount);

	for (int i = 0; i < colorCount; ++i)
	{
		glUniform1i(glGetUniformLocation(shader->id, ("textures[" + std::to_string(i) + "]").c_str()), i);
		glActiveTexture(GL_TEXTURE0 + i);
		if (textureID.at(i))
			Texture::Use(textureID.at(i));
		shader->SetVector3("colors[" + std::to_string(i) + "]", colors[i]);
	}

	shader->SetFloat("minHeight", -heightIntensity);
	shader->SetFloat("maxHeight", heightIntensity);

	for (const auto& it : chunks)
	{
		shader->SetMatrix("model", Mat4::CreateTranslationMatrix({ it.second.GetWorldPos().x, 0.f, it.second.GetWorldPos().y }));

		glBindVertexArray(it.second.GetMesh().GetID());
		glDrawElements(GL_TRIANGLES, (int)it.second.GetMesh().GetNbElements(), GL_UNSIGNED_INT, 0);
	}
}

void Terrain::DisplayOptionWindow()
{
	if (ImGui::Begin("Procedural Generation", nullptr, ImGuiWindowFlags_NoNavInputs))
	{
		if (!isGenerated)
		{
			if (ImGui::Button("Generate"))
			{
				Generate(GameObject::GetGameObjectByTag("Player"));
				Update();
			}
			ImGui::End();
			return;
		}
		ImGui::Checkbox("Always Actualize", &alwaysActualize);
		bool actualized = false;
		actualized |= ImGui::SliderInt("Seed", &seed, 0, 214748364);

		actualized |= ImGui::SliderInt("Chunk Size", &chunkSize, 1, 512);
		actualized |= ImGui::SliderInt("Chunk Vertex Count (LOD)", &chunkVertexCount, 2, 64);

		actualized |= ImGui::SliderFloat("Minimum Height", &minHeight, -100, maxHeight);
		actualized |= ImGui::SliderFloat("Maximum Height", &maxHeight, minHeight, 100);
		actualized |= ImGui::SliderFloat("Height Intensity", &heightIntensity, 1, 200);

		if (ImGui::Button("<"))
			noiseID = (noiseID - 1 + noiseCount) % (noiseCount - 1);
		ImGui::SameLine();
		ImGui::Text("%d/%d", noiseID + 1, noiseParams.size());
		ImGui::SameLine();
		if (ImGui::Button(">"))
			noiseID = (noiseID + 1) % (int)noiseParams.size();
		ImGui::SameLine();

		if (ImGui::Button("Add Noise Layer"))
		{
			actualized = true;
			AddNoiseLayer();
		}
		ImGui::SameLine();
		if (ImGui::Button("Remove Noise Layer"))
		{
			actualized = true;
			DeleteCurrentNoiseLayer();
		}

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
		
		if (ImGui::Button("< "))
			colorID = (colorID - 1 + colorCount) % (colorCount);
		ImGui::SameLine();
		ImGui::Text("%d/%d", colorID + 1, colorCount);
		ImGui::SameLine();
		if (ImGui::Button("> "))
			colorID = (colorID + 1) % (colorCount);
		ImGui::SameLine();
		if (ImGui::Button("Add Color Layer"))
		{
			actualized = true;
			AddColorLayer();
		}
		ImGui::SameLine();
		if (ImGui::Button("Remove Color Layer"))
		{
			actualized = true;
			DeleteCurrentColorLayer();
		}
		ImGui::SliderFloat("Color Height", &colorHeight[colorID], 0.0f, 1.0f);
		ImGui::SliderFloat("Color Blend", &colorBlend[colorID], 0.0f, 1.0f);
		ImGui::SliderFloat("Color Strengh", &colorStrength[colorID], 0.0f, 1.0f);
		ImGui::ColorEdit3("Color", colors[colorID].xyz);
		ImGui::SliderFloat("Texture Scale", &textureScale[colorID], 0.0f, 100.0f);
		ResourcesManager::ResourceChanger<Texture>("Texture", textureID.at(colorID));

		if (alwaysActualize && actualized || ImGui::Button("Actualize"))
			Actualise();

	}
	ImGui::End();
}

void Terrain::AddNoiseLayer()
{
	if (noiseCount >= MAX_NOISE_COUNT)
		return;
	noiseParams.emplace_back(NoiseParam{});
	noiseID = noiseCount++;
}

void Terrain::DeleteCurrentNoiseLayer()
{
	if (noiseCount <= 1)
		return;
	noiseParams.erase(noiseParams.begin() + noiseID);
	noiseID = (noiseID - 1) % --noiseCount;
}

void Terrain::AddColorLayer()
{
	if (colorCount >= MAX_COLOR_COUNT)
		return;
	colors.emplace_back(Vec3{ 0.25f * colorCount, 0.25f * colorCount, 0.25f * colorCount });
	colorHeight.emplace_back(0.25f * colorCount);
	colorBlend.emplace_back(0);
	textureID.emplace_back(nullptr);
	colorStrength.emplace_back(0);
	textureScale.emplace_back(1);
	colorID = colorCount++;
}

void Terrain::DeleteCurrentColorLayer()
{
	if (colorCount <= 1)
		return;
	colors.erase(colors.begin() + colorID);
	colorHeight.erase(colorHeight.begin() + colorID);
	colorBlend.erase(colorBlend.begin() + colorID);
	textureID.erase(textureID.begin() + colorID);
	colorStrength.erase(colorStrength.begin() + colorID);
	textureScale.erase(textureScale.begin() + colorID);
	colorID = (colorID - 1) % --colorCount;
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

	return height / _cca.noiseParams.size();
}

void Chunk::Generate(ChunkCreateArg _cca, bool _reGenerate)
{
	pos = _cca.pos;
	size = _cca.size;
	vertexCount = _cca.vertexCount;

	std::vector<Vertex> vertices;
	vertices.reserve((size_t)vertexCount * (size_t)vertexCount);
	std::vector<int> indices;
	indices.reserve(6 * ((size_t)vertexCount - 1) * ((size_t)vertexCount - 1));

	for (float z = 0.0f; z < vertexCount; z++)
	{
		for (float x = 0.0f; x < vertexCount; x++)
		{
			float height = std::clamp(CalculateHeigt(_cca, x, z) * _cca.heightIntensity, _cca.minHeight, _cca.maxHeight);

			// create vertices
			vertices.push_back(Vertex{ Vec3(x / ((float)vertexCount - 1) * size, height, z / ((float)vertexCount - 1) * size),
										Vec3(0.0f, 1.0f, 0.0f),
										Vec2(x / ((float)vertexCount - 1), z / ((float)vertexCount - 1)) });
		}
	}

	// indices
	for (int z = 0; z < vertexCount - 1; z++)
	{
		for (int x = 0; x < vertexCount - 1; x++)
		{
			int topLeft = (z * vertexCount) + x;
			int topRight = topLeft + 1;
			int bottomLeft = ((z + 1) * vertexCount) + x;
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

	//physxq
	std::vector<PxHeightFieldSample> samples;
	samples.reserve(vertexCount * vertexCount);

	int j = 0;
	int k = 0;
	for (int i = 0; i < vertexCount * vertexCount; ++i)
	{
		samples.push_back({});
		samples[i].height = (physx::PxI16)vertices[j + k].pos.y;
		if (k == vertexCount * (vertexCount - 1))
		{
			j += 1;
			k = 0;
		}
		else
			k += vertexCount;
	}

	PxHeightFieldDesc hfDesc;
	hfDesc.format = PxHeightFieldFormat::eS16_TM;
	hfDesc.nbColumns = vertexCount;
	hfDesc.nbRows = vertexCount;
	hfDesc.samples.data = samples.data();
	hfDesc.samples.stride = sizeof(PxHeightFieldSample);

	PxHeightField* aHeightField = PhysicSystem::cooking->createHeightField(hfDesc,
		PhysicSystem::physics->getPhysicsInsertionCallback());

	PxHeightFieldGeometry hfGeom(aHeightField, PxMeshGeometryFlags(), 1, (float)size / (float)(vertexCount -1),
		(float)size / (float)(vertexCount -1));

	PxTransform t(PxVec3FromVec3(Vec3(pos.x * (float)size, 0, pos.y * (float)size)));
	
	if (actor)
	{
		PhysicSystem::scene->removeActor(*actor);
		actor = nullptr;
	}

	actor = PhysicSystem::physics->createRigidStatic(t);

	actor->userData = this;

	shape = PxRigidActorExt::createExclusiveShape(*actor, hfGeom, _cca.material->data(), 1);
	PhysicSystem::scene->addActor(*actor);

	isGenerated = true;
}

Chunk::~Chunk()
{
	/*if (actor == nullptr || PhysicSystem::scene == nullptr)
		return;

	if (isGenerated)
	{
		if (actor->getScene() == PhysicSystem::scene)
		{
			PhysicSystem::scene->removeActor(*actor);
			actor = nullptr;
		}
	}*/
}