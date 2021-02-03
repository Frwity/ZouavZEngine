#pragma once

#include <vector>


class ScriptSystem
{
private:
	static std::vector<class ScriptComponent*> scripts;

public:
	ScriptSystem() = delete;
	~ScriptSystem() = delete;

	static void AddScript(class ScriptComponent* _script);
	static void RemoveScript(class ScriptComponent* _script);

	static void Begin();
	static void FixedUpdate();
	static void Update();
};