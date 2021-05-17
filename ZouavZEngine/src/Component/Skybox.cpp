#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"
#include "System/ResourcesManager.hpp"
#include <glad/glad.h>
#include <fstream>
#include "Maths/Vec4.hpp"
#include "Maths/Mat4.hpp"
#include "Rendering/Camera.hpp"
#include "Component/Skybox.hpp"
#include "GameObject.hpp"

struct DDSPixelFormat
{
    uint32_t size;
    uint32_t flags;
    uint32_t fourCC;
    uint32_t rgbBitCount;
    uint32_t rBitMask;
    uint32_t gBitMask;
    uint32_t bBitMask;
    uint32_t aBitMask;
};

struct DDSHeader
{
    uint32_t       size;
    uint32_t       flags;
    uint32_t       height;
    uint32_t       width;
    uint32_t       pitchOrLinearSize;
    uint32_t       depth;
    uint32_t       mipMapCount;
    uint32_t       reserved1[11];
    DDSPixelFormat pixelFormat;
    uint32_t       caps;
    uint32_t       caps2;
    uint32_t       caps3;
    uint32_t       caps4;
    uint32_t       reserved2;
};

#define DDS_SURFACE_FLAGS_CUBEMAP 0x00000008 // DDSCAPS_COMPLEX

#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

#define DDS_CUBEMAP_ALLFACES ( DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
                               DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
                               DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ )

void UploadCubemap(const char* _texturePath)
{
    std::ifstream file(_texturePath, std::ios::binary);

    // Parse magic number
    char magic[4] = {};
    file.read((char*)&magic, sizeof(uint32_t));
    if (strncmp(magic, "DDS ", 4) != 0)
    {
        fprintf(stderr, "Not a dds file\n");
        return;
    }

    // Parse header
    DDSHeader header = {};
    file.read((char*)&header, sizeof(DDSHeader));
    file.seekg(header.size - sizeof(DDSHeader), std::ios_base::cur); // Jump to data

    // Abort loading if the texture is not a cubemap...
    if ((header.caps & DDS_SURFACE_FLAGS_CUBEMAP) == 0 ||
        (header.caps2 & DDS_CUBEMAP_ALLFACES) == 0)
    {
        fprintf(stderr, "Not a cubemap or not complete\n");
        return;
    }

    // Abort if the format is not supported
    if (header.pixelFormat.rgbBitCount != 128)
    {
        fprintf(stderr, "Only 128 bits (4 floats) textures supported\n");
        return;
    }

    // Parse each texture level and upload them to GPU
    std::vector<Vec4> buffer(header.width * header.height); // Buffer size for level 0
    for (int i = 0; i < 6; ++i)
    {
        for (uint32_t level = 0; level <= header.mipMapCount; ++level)
        {
            int width = header.width >> level;
            int height = header.height >> level;

            file.read((char*)buffer.data(), width * height * sizeof(Vec4));

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, buffer.data());
        }
    }
}

Skybox::Skybox(class GameObject* _gameObject)
	: Component(_gameObject), cubeMesh(*ResourcesManager::GetResource<Mesh>("Default")), shader(*ResourcesManager::GetResource<Shader>("Skybox"))
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    UploadCubemap("resources/Skybox.dds");
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Skybox::Skybox(class GameObject* _gameObject, const char* _texturePath)
	: Component(_gameObject), cubeMesh(ResourcesManager::GetResource<Mesh>("Default")->get()), shader(ResourcesManager::GetResource<Shader>("Skybox")->get())
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    UploadCubemap(_texturePath);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Skybox::Skybox(const Skybox& _other)
    : Component(_other), cubeMesh{_other.cubeMesh}, shader{_other.shader}
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    UploadCubemap("resources/Skybox.dds");
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Skybox::Draw(const Camera& _camera)
{
    glDepthMask(GL_FALSE);
    glUseProgram(shader->id);

    shader->SetMatrix("projection", _camera.GetProjetionMatrix());
    shader->SetMatrix("view", _camera.GetMatrix().Reversed());

    glBindVertexArray(cubeMesh->GetID());
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

void Skybox::Editor()
{

}

template <class Archive>
static void Skybox::load_and_construct(Archive& _ar, cereal::construct<Skybox>& _construct)
{
    _construct(GameObject::currentLoadedGameObject);
    _ar(cereal::base_class<Component>(_construct.ptr()));
}