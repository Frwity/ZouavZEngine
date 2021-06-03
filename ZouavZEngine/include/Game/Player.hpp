#pragma once

#include "Game/ICharacter.hpp"
#include "Game/ICharacter.hpp"
#include "Maths/Vec2.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/access.hpp"
#include "Component/Animation.hpp"

class Player : public ICharacter
{
private:

	friend class cereal::access;

	class Camera* camera;

	int speed = 3;
	int maxXp = 10;
	int currentXp = 0;
	int attackDamageGain = 1;
	int lifeGain = 5;

	float camSensitivity = 0.1f;
	float rotSpeed = 0.1f;

	bool isJumping = false;
	Animation* animComponent = nullptr;

public:

	Player() = delete;
	Player(class GameObject* _gameobject, std::string _name = "Player");
	void Begin() final;
	void Update() final;

	void OnAddComponent() override;

	void OnContact(class Object* _other, class ShapeCollision* _triggerShape) override;

	void ManageXp(const class Enemy& _enemyKilled);

	void Editor() override;

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