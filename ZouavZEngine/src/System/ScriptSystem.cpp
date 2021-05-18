#include "Component/ScriptComponent.hpp"
#include <imgui.h>
#include "GameObject.hpp"
#include "System/ScriptSystem.hpp"

template<class T> struct componentType { using type = T; };

template <typename T>
bool ComponentButton(std::string _text, bool _onlyOne, GameObject* _gameObjectInspector)
{
	return ComponentButton(_text, _onlyOne, _gameObjectInspector, componentType<T>());
}

# define GENADDCOMPONENT(typeName)\
bool ComponentButton(std::string _text, bool _onlyOne, GameObject* _gameObjectInspector, componentType<typeName>)\
{\
    if ((!_onlyOne || (_onlyOne && !_gameObjectInspector->GetComponent<typeName>())) && ImGui::Button((_text + #typeName).c_str()))\
    {\
	    _gameObjectInspector->AddComponent<typeName>();\
	    return true;\
    }\
    return false;\
}

std::vector<ScriptComponent*> ScriptSystem::scripts;
std::vector<std::function<bool(std::string, bool, GameObject*)>*> ScriptSystem::addComponentsFunctions;

bool ScriptSystem::AddComponents(GameObject* _gameObjectInspector)
{
	for (auto& addComponentFunction : addComponentsFunctions)
	{
		if (addComponentFunction->operator()("Add ", false, _gameObjectInspector))
			return true;
	}

	return false;
}

void ScriptSystem::AddScript(ScriptComponent* _script)
{
	scripts.push_back(_script);
	//addComponentsFunctions.emplace_back(((Player*)_script)->ComponentButton("lameredantonyestsympa", false, nullptr));
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