#include "GameObject.hpp"
#include "Component/Component.hpp"
#include <fstream>
#include <iostream>

Component::Component(GameObject* _gameObject)
	: gameObject(_gameObject)
{
}

void Component::Editor()
{}