#pragma once

#include "Component/ShapeCollision.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"

class Plane: public ShapeCollision
{
public:
	Plane() = delete;
	Plane(GameObject* _gameObject);
	~Plane();

	template <class Archive>
	void serialize(Archive& _ar)
	{
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<Plane>& _construct)
	{
		_construct(GameObject::currentLoadedGameObject);
	}
};

CEREAL_REGISTER_TYPE(Plane)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ShapeCollision, Plane)