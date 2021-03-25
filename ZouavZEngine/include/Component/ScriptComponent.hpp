#pragma once

#include "Component/Component.hpp"

class ScriptComponent: public Component
{
private:

public:
	ScriptComponent() = delete;
	ScriptComponent(class GameObject* _gameObject);
	virtual ~ScriptComponent();

	virtual void Begin() {};
	virtual void FixedUpdate() {};
	virtual void Update() {};
	virtual void OnDestroy() {};
};