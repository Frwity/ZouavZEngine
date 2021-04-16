#pragma once

#include "Maths/Vec4.hpp"

class Material
{
public:
	class Shader* shader = nullptr;
	class Texture* texture = nullptr;
	Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Material();
	Material(class Shader* _shader, class Texture* _texture, const Vec4& _color);
	~Material() = default;
};