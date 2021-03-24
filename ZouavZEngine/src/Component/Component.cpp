#include "GameObject.hpp"
#include "Component/Component.hpp"
#include <fstream>
#include <iostream>

std::vector<std::string> Component::componentsList = {"AudioBroadcaster", "AudioListener", "Light", "MeshRenderer"};

Component::Component(GameObject* _gameObject)
	: gameObject(_gameObject)
{
}

const std::vector<std::string>& Component::GetComponentsList()
{
	return componentsList;
}

void Component::Editor()
{}