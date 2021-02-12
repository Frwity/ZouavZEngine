#pragma once
#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include <unordered_map>


struct ChunkCreateArg
{
	Vec2 pos;

	int size;
	int vertexCount;

	int octaves;
	float persistance;
	float lacunarity;

	float minHeight;
	float maxHeight;
	float heightIntensity;
};

class Chunk
{
private:
	Mesh mesh{};

	Vec2 pos{};

	int size = 0;
public:

	Chunk() = default;

	void Generate(ChunkCreateArg _cca, bool _reGenerate);

	const Mesh& GetMesh() { return mesh; }

	Vec2 GetPos() { return pos; }
	float GetSize() { return size; }
	Vec2 GetWorldPos() { return pos * size; }
};

class Terrain
{
private:
	std::unordered_map<std::string, Chunk> chunks;

	Shader* shader{};


public:
	int chunkCount = 3;

	int chunkSize = 128;
	int chunkVertexCount = 16;

	int octaves = 1;
	float persistance = 1.0f;
	float lacunarity = 1.0f;

	float minHeight = 10.0f;
	float maxHeight = 30.0f;
	float heightIntensity = 30.0f;

	bool alwaysActualize = false;

	Terrain();

	void Generate();

	void Actualise();

	void Draw(const std::vector<class Light*>& _lights);
};