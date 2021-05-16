#pragma once

#include "Component/ScriptComponent.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"
#include "Prefab.hpp"


class Player : public ScriptComponent
{
private:

	friend class cereal::access;

	class RigidBody* rb = nullptr;

	Prefab skull;

	Vec3 pos;

	int speed = 3;
	int vie = 10;
	float distance = 1.f;

public:

	Player() = delete;
	Player(class GameObject* _gameobject);
	Component* Clone() const override { return new Player(*this); }

	void Editor() override;

	void Begin() final;
	void Update() final;

	const char* GetComponentName() override { return "Player"; }

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(speed, vie, distance, skull);
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


		_ar(_construct->skull);
	}
};

CEREAL_REGISTER_TYPE(Player)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Player)