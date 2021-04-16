#pragma once

#include "Component/ShapeCollision.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"

class CapsuleCollision: public ShapeCollision
{
public:
	float radius;
	float halfHeight;

	CapsuleCollision(GameObject* _gameObject, float _radius = 1.0f, float _halfHeight = 1.0f);
	~CapsuleCollision();

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(radius, halfHeight);
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<CapsuleCollision>& _construct)
	{
		float _radius;
		float _halfHeight;
		_ar(_radius, _halfHeight);
		_construct(GameObject::currentLoadedGameObject, _radius, _halfHeight);
	}
};

CEREAL_REGISTER_TYPE(CapsuleCollision)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ShapeCollision, CapsuleCollision)