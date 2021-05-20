#include "Component/ScriptComponent.hpp"
#include "System/ScriptSystem.hpp"

ScriptComponent::ScriptComponent(class GameObject* _gameObject, std::string _name)
	: Component(_gameObject, _name)
{
	ScriptSystem::AddScript(this);
}

ScriptComponent::~ScriptComponent()
{
	ScriptSystem::RemoveScript(this);
}