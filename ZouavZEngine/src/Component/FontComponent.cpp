#include "Rendering/Camera.hpp"
#include "GameObject.hpp"
#include "Maths/Mat4.hpp"
#include "System/ResourcesManager.hpp"
#include "Component/FontComponent.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"



FontComponent::FontComponent(class GameObject* _gameObject)
	: Component( _gameObject), font{ *ResourcesManager::GetResource<Font>("Default") }, shader { *ResourcesManager::GetResource<Shader>("FontShader") }
{
	ChangeText(text.c_str(), text.size());
}

FontComponent::FontComponent(GameObject* _gameObject, std::shared_ptr<Font>& _font)
	: Component(_gameObject), font{ _font }, shader{ *ResourcesManager::GetResource<Shader>("FontShader") }
{
	ChangeText(text.c_str(), text.size());
}

void FontComponent::Editor()
{
	std::string tempText = text;
	if (ImGui::InputText("Text : ", tempText.data(), 256))
	{
		text = tempText;
		ChangeText(text.c_str(), text.size());
	}
	if (ImGui::InputFloat("Font Size :", &fontSize, 0.1f))
		ChangeText(text.c_str(), text.size());
	ImGui::ColorEdit4("Color : ", &color.w);
	ImGui::InputFloat3("Position : ", &position.x);
	ImGui::Checkbox("Billboard :", &billBoard);
	ImGui::Checkbox("Depth Test (TODO) :", &depthTest);
}

void FontComponent::Draw(const Mat4& _transform, const class Camera& _camera)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	shader->Use();
	glActiveTexture(GL_TEXTURE0);
	font->GetTexture().Use();
	Mat4 matrixCamera = _camera.GetMatrix();

	shader->SetMatrix("view", matrixCamera.Reversed());
	shader->SetMatrix("projection", _camera.GetProjetionMatrix());
	shader->SetMatrix("model", Mat4::CreateTRSMatrix(GetGameObject().WorldPosition(), GetGameObject().WorldRotation(), GetGameObject().WorldScale()) * Mat4::CreateTranslationMatrix(position));
	shader->SetVector3("viewPos", matrixCamera.Accessor(0, 3), matrixCamera.Accessor(1, 3), matrixCamera.Accessor(2, 3));
	shader->SetVector4("color", color);

	glBindVertexArray(mesh.GetID());
	glDrawElements(GL_TRIANGLES, (GLsizei)mesh.GetNbElements(), GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void FontComponent::ChangeText(const char* _newText, int _size)
{
	if (_size < 1)
		return;
	float curserX = 0.f;
	float curserY = 0.f;
	std::vector<Vertex> vertices;
	vertices.reserve(_size * 4);
	std::vector<int> indices;
	indices.reserve(_size * 6);

	int i = 0;
	while (_newText[i] != '\0')
	{
		const int ascii = (int)_newText[i];
		if (ascii == ' ')
		{
			curserX += font->GetMetaData().GetSpaceWidth() * fontSize;
			++i;
			continue;
		}
		if (ascii == '\n')
		{
			curserX = 0.0f;
			curserY += 0.03f * fontSize; // like the other TODO in Font.cpp
			++i;
			continue;
		}
		const Character* character = font->GetMetaData().GetCharacter(ascii);

		const float x = curserX + (character->GetxOffset() * fontSize);
		const float y = curserY + (character->GetyOffset() * fontSize);
		const float maxX = x + (character->GetSizeX() * fontSize);
		const float maxY = y + (character->GetSizeY() * fontSize);
		const float properX = (2.f * x) - 1.f;
		const float properY = (-2.f * y) + 1.f;
		const float properMaxX = (2.f * maxX) - 1.f;
		const float properMaxY = (-2.f * maxY) + 1.f;
		
		indices.push_back(vertices.size());
		indices.push_back(vertices.size() + 3 );
		indices.push_back(vertices.size() + 2 );
		indices.push_back(vertices.size());
		indices.push_back(vertices.size() + 2 );
		indices.push_back(vertices.size() + 1 );

		vertices.push_back(Vertex{ Vec3(properX, properY, 0.0f),
									Vec3(0.0f, 0.0f, 1.0f),
									Vec2(character->GetxTextureCoord(), 1 - character->GetyTextureCoord()) });
		vertices.push_back(Vertex{ Vec3(properX, properMaxY, 0.0f),
									Vec3(0.0f, 0.0f, 1.0f),
									Vec2(character->GetxTextureCoord(), 1 - character->GetYMaxTextureCoord()) });
		vertices.push_back(Vertex{ Vec3(properMaxX, properMaxY, 0.0f),
									Vec3(0.0f, 0.0f, 1.0f),
									Vec2(character->GetXMaxTextureCoord(), 1 - character->GetYMaxTextureCoord()) });
		vertices.push_back(Vertex{ Vec3(properMaxX, properY, 0.0f),
									Vec3(0.0f, 0.0f, 1.0f),
									Vec2(character->GetXMaxTextureCoord(), 1 - character->GetyTextureCoord()) });

		curserX += character->GetxAdvance() * fontSize;

		++i;
	}

	if (isGenerated)
		mesh.ChangeSizeAndData(vertices.data(), vertices.size(), indices.data(), indices.size());
	else
		mesh.InitMesh(vertices.data(), vertices.size(), indices.data(), indices.size());

	isGenerated = true;
}

template <class Archive>
static void FontComponent::load_and_construct(Archive& _ar, cereal::construct<FontComponent>& _construct)
{
	std::string fontName;
	std::string fontPath;
	bool isfontDeletebale;
	_ar(fontName, isfontDeletebale, fontPath);
	_construct(GameObject::currentLoadedGameObject, *ResourcesManager::AddResourceFont(fontName, isfontDeletebale, fontPath.c_str()));
}