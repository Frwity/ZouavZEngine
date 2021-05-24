#pragma once

#include "Game/ICharacter.hpp"
#include "Game/ICharacter.hpp"
#include "Maths/Vec2.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"

class Player : public ICharacter
{
private:

	friend class cereal::access;

	class RigidBody* rb{ nullptr };
	class BoxCollision* attackCollision;
	class GameObject* playerMesh;
	class GameObject* camera;

	int speed = 3;

	float attackDuration = 0.5f;
	float attackCooldown = 1.0f;
	float timerAttackDuration = 0.0f;
	float timerAttackCooldown = 0.0f;

	bool isJumping = false;

public:

	Player() = delete;
	Player(class GameObject* _gameobject, std::string _name = "Player");
	void Begin() final;
	void Update() final;

	void OnAddComponent() override;

	void OnTrigger(class GameObject* _other, class ShapeCollision* _triggerShape) override;

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