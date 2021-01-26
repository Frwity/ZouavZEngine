#include <vector>
#include <iostream>

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
		
	const aiScene* scene = importer.ReadFile(path,
		                                    aiProcess_Triangulate |
		                                    aiProcess_GenSmoothNormals |
											aiProcess_FlipUVs);

    if (!scene)
        std::cout << "Mesh load failed!: " << path << std::endl;

    const aiMesh* mesh = scene->mMeshes[0];

    std::vector<Vertex> vertices;
	std::vector<int> indices;

    const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) 
	{
		const aiVector3D* pPos = &(mesh->mVertices[i]);
		const aiVector3D* pNormal = mesh->HasNormals() ? &(mesh->mNormals[i]) : &aiZeroVector;
		const aiVector3D* pTexCoord;/* = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &aiZeroVector;*/

		Vec2 vec;

		if (mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
		{
			vec.x = i / 10 % 10;
			vec.y = i / 10 % 10;
			//vertex.TexCoords = vec;
		}
		else
			 vec = { 0, 0 };

		Vertex vert{Vec3(pPos->x, pPos->y, pPos->z),
					Vec3(pNormal->x, pNormal->y, pNormal->z),
					vec};

		vertices.push_back(vert);
	}

	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
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
	
	InitMesh(vertices.data(), vertices.size(), indices.data(), indices.size());
}
