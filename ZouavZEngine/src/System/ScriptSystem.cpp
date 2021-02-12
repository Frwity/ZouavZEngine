#include "Component/ScriptComponent.hpp"
#include "System/ScriptSystem.hpp"

std::vector<class ScriptComponent*> ScriptSystem::scripts;

void ScriptSystem::AddScript(class ScriptComponent* _script)
{
	scripts.push_back(_script);
}

void ScriptSystem::RemoveScript(class ScriptComponent* _script)
{
	for (auto it = scripts.begin(); it != scripts.end(); )
	{
		if (*it == _script)
			it = scripts.erase(it);
		else
			++it;
	}
}

void ScriptSystem::Begin()
{
	for (ScriptComponent* script : scripts)
		script->Begin();
}

void ScriptSystem::FixedUpdate()
{
	for (ScriptComponent* script : scripts)
		script->FixedUpdate();
}

void ScriptSystem::Update()
{
	for (ScriptComponent* script : scripts)
		script->Update();
}