#pragma once
#include "Resource.hpp"

class Texture: public Resource
{
    friend class Engine;
private:
    static const Texture* errorTexture;
public:

    unsigned int textureID = 0;
    Texture(const std::string& _name, const char* textPath);
    ~Texture() = default;

    static void Use(Texture* _texture);
};
