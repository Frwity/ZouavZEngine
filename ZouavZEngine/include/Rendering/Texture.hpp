#pragma once
#include "Resource.hpp"

class Texture: public Resource
{
public:

    unsigned int texture = 0;
    Texture(const std::string& _name, const char* textPath);
    ~Texture() = default;
    void Use();
};
