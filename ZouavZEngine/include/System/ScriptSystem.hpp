#pragma once

#include <vector>
#include <functional>

class ScriptSystem
{
private:
	static std::vector<class ScriptComponent*> scripts;
	static std::vector<std::function<bool(std::string, bool, class GameObject*)>*> addComponentsFunctions;

public:
	ScriptSystem() = delete;
	~ScriptSystem() = delete;

	static bool AddComponents(class GameObject* _gameObjectInspector);

	static void AddScript(class ScriptComponent* _script);
	static void RemoveScript(class ScriptComponent* _script);

	static void Begin();
	static void FixedUpdate();
	static void Update();
};