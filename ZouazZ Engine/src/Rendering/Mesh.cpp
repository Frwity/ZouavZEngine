#include <vector>
#include <iostream>
#include <assert.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Maths/Vec2.hpp"
#include "Maths/Vec3.hpp"
#include "Rendering/Mesh.hpp"


Mesh::Mesh(const char* path)
{
    Assimp::Importer importer;
		
	const aiScene* scene = importer.ReadFile(path, aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_PreTransformVertices |
		aiProcess_CalcTangentSpace |
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_FixInfacingNormals |
		aiProcess_FindInvalidData |
		aiProcess_ValidateDataStructure | 0);
    if (!scene)
        std::cout << "Mesh load failed!: " << path << std::endl;

    std::vector<Vertex> vertices;
	std::vector<int> indices;

    const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);


	for (unsigned int m = 0; m < scene->mNumMeshes; ++m)
	{
		const aiMesh* mesh = scene->mMeshes[m];
		for (unsigned int j = 0; j < mesh->mNumFaces; ++j) 
		{
			auto const& face = mesh->mFaces[j];
			for (int k = 0; k < 3; ++k) 
			{
				auto const& vertex = mesh->mVertices[face.mIndices[k]];
				auto const& normal = /*mesh->mNormals[face.mIndices[k]]*/ Vec3(0.0f, 0.0f, 0.0f);
				auto const& uv = mesh->HasTextureCoords(0) ? /*Vec2(mesh->mTextureCoords[0][face.mIndices[k]].x, mesh->mTextureCoords[0][face.mIndices[k]].y)*/ Vec2(0.0f, 0.0f) : Vec2(0.0f, 0.0f);

				vertices.push_back(Vertex{	Vec3(vertex.x, vertex.y, vertex.z),
											Vec3(normal.x, normal.y, normal.z),
											uv });
			}
		}

		for(unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace& face = mesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}
	}

	InitMesh(vertices.data(), vertices.size(), indices.data(), indices.size());
}

void Mesh::InitMesh(Vertex* vertices, unsigned int vertSize, int* indices, unsigned int indicesSize)
{
	nbElements = indicesSize;

    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)3);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)6);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::CreateQuad()
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	Vertex vert;

	vert = {Vec3(-0.5f, -0.5f, 0.f),
			Vec3(0.f, 0.f, 1.f),
			Vec2(0.f, 0.f)};
	vertices.push_back(vert);

	vert = {Vec3(0.5f, -0.5f, 0.f),
			Vec3(0.f, 0.f, 1.f),
			Vec2(1.f, 0.f)};
	vertices.push_back(vert);

	vert = {Vec3(0.5f, 0.5f, 0.f),
			Vec3(0.f, 0.f, 1.f),
			Vec2(1.f, 1.f)};
	vertices.push_back(vert);

	vert = {Vec3(-0.5f, 0.5f, 0.f),
			Vec3(0.f, 0.f, 1.f),
			Vec2(0.f, 1.f)};
	vertices.push_back(vert);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	
	vert = { Vec3(-0.5f, -0.5f, 1.f),
		Vec3(0.f, 0.f, 1.f),
		Vec2(0.f, 0.f) };
	vertices.push_back(vert);

	vert = { Vec3(0.5f, -0.5f, 1.f),
			Vec3(0.f, 0.f, 1.f),
			Vec2(1.f, 0.f) };
	vertices.push_back(vert);

	vert = { Vec3(0.5f, 0.5f, 1.f),
			Vec3(0.f, 0.f, 1.f),
			Vec2(1.f, 1.f) };
	vertices.push_back(vert);

	vert = { Vec3(-0.5f, 0.5f, 1.f),
			Vec3(0.f, 0.f, 1.f),
			Vec2(0.f, 1.f) };
	vertices.push_back(vert);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(4);
	indices.push_back(6);
	indices.push_back(7);

	InitMesh(vertices.data(), vertices.size(), indices.data(), indices.size());
}
