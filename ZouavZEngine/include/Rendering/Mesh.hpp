#pragma once

#include "Resource.hpp"
#include <vector>
#include "Maths/Vec3.hpp"
#include "Maths/Vec2.hpp"

struct Vertex
{
    Vec3 pos;
    Vec3 normal;
    Vec2 texCoord;
};

class Mesh : public Resource
{
private:
    unsigned int VBO, EBO = 0;
    unsigned int VAO = 0;
    size_t nbElements = 0;

public:
    Mesh(const std::string& _name);
    Mesh(const std::string& _name, const char* path);
    ~Mesh();

    void RemoveFromResourcesManager() override;

    void InitMesh(Vertex* vertices, size_t vertSize, int* indices, size_t indicesSize);
    void ChangeSizeAndData(Vertex* vertices, size_t vertSize, int* indices, size_t indicesSize);
    void CreateCube();
  
    unsigned int GetID() const
    {
        return VAO;
    }

    size_t GetNbElements() const
    {
        return nbElements;
    }
};