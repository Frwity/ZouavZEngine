#include "System/ResourcesManager.hpp"
#include "Rendering/Material.hpp"

Material::Material()
	: shader{ResourcesManager::GetResource<Shader>("Default")}, texture{ ResourcesManager::GetResource<Texture>("Default") }
{}

Material::Material(class Shader* _shader, class Texture* _texture, const Vec4& _color)
	: shader{_shader}, texture{_texture}, color{_color}
{}