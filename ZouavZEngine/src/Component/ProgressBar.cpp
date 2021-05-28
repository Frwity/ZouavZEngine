#include "GameObject.hpp"
#include <imgui.h>
#include <algorithm>
#include <glad/glad.h>
#include "System/ResourcesManager.hpp"
#include "Rendering/Camera.hpp"
#include "Component/ProgressBar.hpp"

ProgressBar::ProgressBar(GameObject* _gameObject, std::string _name)
	: Component(_gameObject, _name), shader{*ResourcesManager::GetResource<Shader>("ProgressBar")}
{
    std::vector<Vertex> vertices;
	vertices.push_back(Vertex{ Vec3(0.5, 0.5, 0.0f),
								Vec3(0.0f, 0.0f, 1.0f),
								Vec2(0.0f, 0.0f) });
	vertices.push_back(Vertex{ Vec3(0.5, -0.5, 0.0f),
								Vec3(0.0f, 0.0f, 1.0f),
								Vec2(0.0f, 0.0f) });
	vertices.push_back(Vertex{ Vec3(-0.5, -0.5, 0.0f),
								Vec3(0.0f, 0.0f, 1.0f),
								Vec2(0.0f, 0.0f) });
	vertices.push_back(Vertex{ Vec3(-0.5, 0.5, 0.0f),
								Vec3(0.0f, 0.0f, 1.0f),
								Vec2(0.0f, 0.0f) });

	std::vector<int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);
	mesh.InitMesh(vertices.data(), vertices.size(), indices.data(), indices.size());
}

void ProgressBar::Draw(const Camera& _camera)
{
	if (currentValue && maxValue)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		shader->Use();

		shader->SetMatrix("projection", Mat4::CreateTranslationMatrix(Vec3{ -1.f + (float)_camera.GetHeight() / (float)_camera.GetWidth() , 0.0f, 0.0f } + pos / 100.f)
										* Mat4::CreateScaleMatrix({ (float)_camera.GetHeight() / (float)_camera.GetWidth(), 1.f, 1.f }));

		shader->SetFloat("sizeFilled", (*currentValue / *maxValue) * size.x);
		shader->SetVector4("backgroundColor", backgroundColor);
		shader->SetVector4("fillColor", fillColor);

		glBindVertexArray(mesh.GetID());
		glDrawElements(GL_TRIANGLES, (GLsizei)mesh.GetNbElements(), GL_UNSIGNED_INT, 0);

		glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);
	}
}

void ProgressBar::Editor()
{
    ImGui::ColorEdit4("Background Color : ", &backgroundColor.x);
    ImGui::ColorEdit4("Fill Color : ", &fillColor.x);
}

template <class Archive>
static void ProgressBar::load_and_construct(Archive& _ar, cereal::construct<ProgressBar>& _construct)
{
    _construct(GameObject::currentLoadedGameObject);
    _ar(cereal::base_class<Component>(_construct.ptr()));
}