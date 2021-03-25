#pragma once

#include "Component/ScriptComponent.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

class Player : public ScriptComponent
{
private:

	friend class cereal::access;

	int speed = 3;
	int vie = 10;
	float distance = 1.f;


public:

	Player() = delete;
	Player(class GameObject* _gameobject);
	void Begin() final;
	void Update() final;

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(speed, vie, distance);
	}
};

//
//CEREAL_REGISTER_TYPE(Player)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Player)