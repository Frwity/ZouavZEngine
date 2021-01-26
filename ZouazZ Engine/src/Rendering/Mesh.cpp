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

    for(unsigned int i = 0; i < mesh->mNumVertices - 1; i++) 
	{
		const aiVector3D* pPos = &(mesh->mVertices[i]);
		const aiVector3D* pNormal = mesh->HasNormals() ? &(mesh->mNormals[i]) : &aiZeroVector;
		const aiVector3D* pTexCoord;/* = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &aiZeroVector;*/

		Vec2 vec;

		if (mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
		{
			vec.x = &mesh->mTextureCoords[0][i].x ? mesh->mTextureCoords[0][i].x : 0;
			vec.y = &mesh->mTextureCoords[0][i].y ? mesh->mTextureCoords[0][i].y : 0;
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

	InitMesh(&vertices[0], vertices.size(), (int*)&indices[0]);
}

void Mesh::InitMesh(Vertex* vertices, int vertSize, int* indices)
{
	unsigned int VBO, EBO;

	glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)3);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)6);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
}