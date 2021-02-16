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
		
	//const aiScene* scene = importer.ReadFile(path, aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_PreTransformVertices |
	//	aiProcess_CalcTangentSpace |
	//	aiProcess_GenSmoothNormals |
	//	aiProcess_Triangulate |
	//	aiProcess_FixInfacingNormals |
	//	aiProcess_FindInvalidData |
	//	aiProcess_ValidateDataStructure | 0);
	const aiScene* scene = importer.ReadFile(path,	aiProcess_Triangulate | 
													aiProcess_FindInvalidData | 
													aiProcess_ValidateDataStructure );
    if (!scene)
        std::cout << "Mesh load failed!: " << path << std::endl;

    std::vector<Vertex> vertices;
	std::vector<int> indices;

	const aiMesh* mesh = scene->mMeshes[0];
	int test = scene->mNumMeshes;
	for (unsigned int v = 0; v < mesh->mNumVertices; v++)
	{
		vertices.push_back(Vertex{	Vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z),
									mesh->HasNormals() ? Vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z) : Vec3(),
									mesh->HasTextureCoords(0) ? Vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y) : Vec2()});

	}

 	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(int), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Mesh::ChangeSizeAndData(Vertex* vertices, unsigned int vertSize, int* indices, unsigned int indicesSize)
{
	nbElements = indicesSize;
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(int), indices, GL_STATIC_DRAW);


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
