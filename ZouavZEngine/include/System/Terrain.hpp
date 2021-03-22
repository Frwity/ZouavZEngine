



#pragma once
#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"

#include <FastNoiseLite.h>

#include <unordered_map>


#define MAX_NOISE_COUNT 4
#define MAX_COLOR_COUNT 8 // need to change the TerrainShader.vs define too // TODO change only one

struct NoiseParam
{
	FastNoiseLite::NoiseType noiseType = FastNoiseLite::NoiseType::NoiseType_Perlin;
	float frequency = 0.01f;
	FastNoiseLite::FractalType fractalType = FastNoiseLite::FractalType::FractalType_None;
	int octaves = 1;
	float lacunarity = 2.0f;
	float gain = 0.5f;
	float weightedStrength = 0.0f;
	float pingPongStength = 2.0f;
};

struct ChunkCreateArg
{
	Vec2 pos;

	int size;
	int vertexCount;

	int seed;

	std::vector<NoiseParam> noiseParams;

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
	~Chunk() = default;

	float CalculateHeigt(ChunkCreateArg _cca, float _x, float _z);
	void Generate(ChunkCreateArg _cca, bool _reGenerate);

	const Mesh& GetMesh() { return mesh; }

	Vec2 GetPos() { return pos; }
	int GetSize() { return size; }
	Vec2 GetWorldPos() { return pos * (float)size; }
};

class Terrain
{
private:
	std::unordered_map<std::string, Chunk> chunks;

	Shader* shader{};

public:

	// infinite variable
	
	class GameObject* actualizer = nullptr;

	float chunkDistanceRadius = 512;

	// chunk variable

	int chunkSize = 128;
	int chunkVertexCount = 16;

	int seed = 0;

	int noiseID = 0;

	std::vector<NoiseParam> noiseParams;

	float minHeight = -10.0f;
	float maxHeight = 30.0f;
	float heightIntensity = 30.0f;

	// texture and color variable

	int colorCount = 0;
	std::vector<Vec3> colors;
	std::vector<float> colorHeight;
	std::vector<float> colorBlend;

	std::vector<std::string> textureNames;
	std::vector<class Texture*> textureID;

	// editor variable

	bool alwaysActualize = true;

	Terrain();

	void Generate(class GameObject* _actualizer = nullptr);
	void Actualise();

	void Update();
	void Draw(const std::vector<class Light*>& _lights, const class Camera& _camera);

	void DisplayOptionWindow();

	void AddNoiseLayer();
	void DeleteCurrentNoiseLayer();

	void AddColor();

};
