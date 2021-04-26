#pragma once

#include "Component/ShapeCollision.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"

class BoxCollision: public ShapeCollision
{
public:
	Vec3 halfExtends;

	BoxCollision(GameObject* _gameObject, Vec3 _halfExtends = { 0.5f, 0.5f, 0.5f }, bool _isTrigger = false);
	~BoxCollision();

	void Editor() override;
	void UpdateExtends(const Vec3& v);

	const char* GetComponentName() override { return "BoxCollision"; }

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