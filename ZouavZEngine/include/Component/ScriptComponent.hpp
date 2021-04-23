#pragma once

#include "Component/Component.hpp"

class ScriptComponent: public Component
{
private:

public:
	ScriptComponent() = delete;
	ScriptComponent(class GameObject* _gameObject);
	virtual ~ScriptComponent();

	const char* GetComponentName() override { return "ScriptComponent"; }

	virtual void Begin() {};
	virtual void FixedUpdate() {};
	virtual void Update() {};
	virtual void OnDestroy() {};
	virtual void OnTrigger(class GameObject* _other) {};
	virtual void OnContact(class GameObject* _other) {};
};