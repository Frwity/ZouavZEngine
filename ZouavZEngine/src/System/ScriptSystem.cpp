#include "Component/ScriptComponent.hpp"
#include "System/ScriptSystem.hpp"

std::vector<ScriptComponent*> ScriptSystem::scripts;

void ScriptSystem::AddScript(ScriptComponent* _script)
{
	scripts.push_back(_script);
}

void ScriptSystem::RemoveScript(ScriptComponent* _script)
{
	for (auto it = scripts.begin(); it != scripts.end(); )
	{
		if (*it == _script)
		{
			scripts.erase(it);
			return;
		}
		else
			++it;
	}
}

void ScriptSystem::Begin()
{
	for (ScriptComponent* script : scripts)
		if (script->IsActive())
			script->Begin();
}

void ScriptSystem::FixedUpdate()
{
	for (ScriptComponent* script : scripts)
		if (script->IsActive())
			script->FixedUpdate();
}

void ScriptSystem::Update()
{
	for (ScriptComponent* script : scripts)
		if (script->IsActive())
			script->Update();
}