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
    unsigned int VAO;
    unsigned int nbPoly;

public:
    Mesh();
    Mesh(const char* path);
    ~Mesh();

    void loadOBJ(const char* path, std::vector<Vec3>& verticesCoords, std::vector<Vec3>& verticesColors, std::vector<Vec3>& verticesNormals);

    unsigned int getID() const
    {
        return VAO;
    }
    unsigned int getNbPoly() const
    {
        return nbPoly;
    }

    void InitMesh(Vertex* vertices, int vertSize, int* indices);

};