#pragma once

#include "Component/ShapeCollision.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"

class BoxCollision: public ShapeCollision
{
public:
	BoxCollision(GameObject* _gameObject);
	~BoxCollision();

	template <class Archive>
	void serialize(Archive& _ar)
	{
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<BoxCollision>& _construct)
	{
		_construct(GameObject::currentLoadedGameObject);
	}
};

CEREAL_REGISTER_TYPE(BoxCollision)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ShapeCollision, BoxCollision)