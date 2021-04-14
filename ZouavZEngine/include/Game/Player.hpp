#pragma once

#include "Component/ScriptComponent.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"



class Player : public ScriptComponent
{
private:

	friend class cereal::access;

	class RigidBody* rb;

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

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<Player>& _construct)
	{
		int _speed;
		int _vie;
		float _distance;

		_ar(_speed, _vie, _distance);

		_construct(GameObject::currentLoadedGameObject);
		_construct->speed = _speed;
		_construct->vie = _vie;
		_construct->distance = _distance;
	}
};


CEREAL_REGISTER_TYPE(Player)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Player)