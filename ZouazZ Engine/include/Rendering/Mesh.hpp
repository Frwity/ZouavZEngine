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
    unsigned int VBO, EBO;
    unsigned int VAO;
    unsigned int nbElements;

public:
    Mesh() = default;
    Mesh(const char* path);
    ~Mesh();

    void InitMesh(Vertex* vertices, unsigned int vertSize, int* indices, unsigned int indicesSize);
    void CreateQuad();

    unsigned int GetID() const
    {
        return VAO;
    }
    unsigned int GetNbElements() const
    {
        return nbElements;
    }


};