#include "GameObject.hpp"
#include <fstream>
#include <iostream>
#include "imgui.h"
#include "Component/Component.hpp"

Component::Component(GameObject* _gameObject)
	: gameObject(_gameObject)
{
}

void Component::Editor()
{

}

bool Component::EditorCollapsingHeader(const char* _name, std::function<void()> _f)
{
	bool open = true;
	if (ImGui::CollapsingHeader(_name, &open, ImGuiTreeNodeFlags_DefaultOpen))
	{
		_f();
		ImGui::NewLine();
	}
	return open;
}

void Component::DeleteFromGameObject()
{
	gameObject->RemoveComponent(this);
}

template <class Archive>
static void Component::load_and_construct(Archive& _ar, cereal::construct<Component>& _construct)
{
	_construct(GameObject::currentLoadedGameObject);
}