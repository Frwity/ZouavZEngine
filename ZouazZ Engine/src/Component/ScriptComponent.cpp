#include "Component/ScriptComponent.hpp"
#include "System/ScriptSystem.hpp"

ScriptComponent::ScriptComponent(class GameObject* _gameObject)
	: Component(_gameObject)
{
	ScriptSystem::AddScript(this);
}

ScriptComponent::~ScriptComponent()
{
	ScriptSystem::RemoveScript(this);
}