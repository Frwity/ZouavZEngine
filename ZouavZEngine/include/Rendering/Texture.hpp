#pragma once
#include "Resource.hpp"

class Texture: public Resource
{
public:

    unsigned int texture;
    Texture() = default;
    ~Texture() = default;
    Texture(const char* textPath);
    void Use();
};
