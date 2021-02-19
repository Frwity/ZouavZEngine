#pragma once

#include "Component/ScriptComponent.hpp"

class Player : public ScriptComponent
{
private:
	int speed = 3;
public:

	Player(class GameObject* _gameobject);

	void Begin() final;
	void Update() final;
};