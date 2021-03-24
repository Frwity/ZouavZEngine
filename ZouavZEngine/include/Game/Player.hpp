#pragma once

#include "Component/ScriptComponent.hpp"

#include "cereal/archives/json.hpp"
#include "cereal/access.hpp"

class Player : public ScriptComponent
{
private:

	friend class cereal::access;

	int speed = 3;
	int vie;
	float distance;
public:

	Player(class GameObject* _gameobject);

	void Begin() final;
	void Update() final;

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(speed, vie, distance);
	}
};