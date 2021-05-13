#pragma once

#include "Rendering/Font.hpp"
#include "Rendering/Texture.hpp"
#include "Rendering/Mesh.hpp"
#include "Component/Component.hpp"
#include "Maths/Vec3.hpp"

#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"

#include <string>
#include <memory>

class FontComponent : public Component
{
private:
	std::shared_ptr<Font> font;
	std::shared_ptr<Shader> shader;

	Mesh mesh{ "TextMesh" };

	std::string text{ "Default Text" };
	Vec4 color{ 0.f, 0.f, 0.f, 1.0f };
	Vec3 position{ 0.f, 0.f, 0.f};
	float fontSize {4.0f};
	bool billBoard = false;
	bool depthTest = false; // TODO if needed have to render all the text in after the scene rendering


	bool isGenerated = false;

public:
	FontComponent() = delete;
	FontComponent(class GameObject* _gameObject);
	FontComponent(class GameObject* _gameObject, std::shared_ptr<Font>& _font);

	virtual ~FontComponent() = default;

	const std::string& GetText() const { return text; }

	void ChangeText(const char* _newText, int _size);

	void Draw(const class Mat4& _transform, const class Camera& _camera);

	void Editor() override;

	const char* GetComponentName() override { return "FontComponent"; }

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(font->GetName(), font->IsDeletable(), font->GetPaths()[0]);
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<FontComponent>& _construct);
};

CEREAL_REGISTER_TYPE(FontComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, FontComponent)