#pragma once

#include "Game/ICharacter.hpp"
#include "Component/ScriptComponent.hpp"
#include "Maths/Vec2.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

class Player : public ICharacter
{
private:

	friend class cereal::access;

	class RigidBody* rb{ nullptr };

	int speed = 3;
	float xCameraAngle = 0.0f;
	
	Vec2 oldMousePos{ 0.0f, 0.0f };

public:

	Player() = delete;
	Player(class GameObject* _gameobject);
	Component* Clone() const override { return new Player(*this); }
	void Begin() final;
	void Update() final;

	const char* GetComponentName() override { return "Player"; }

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(speed);

		ar(cereal::base_class<ICharacter>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<Player>& _construct)
	{
		_construct(GameObject::currentLoadedGameObject);
		_ar(_construct->speed);

		_ar(cereal::base_class<ICharacter>(_construct.ptr()));
	}
};


CEREAL_REGISTER_TYPE(Player)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ICharacter, Player)