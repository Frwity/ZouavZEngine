#pragma once

#include "Component/ShapeCollision.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include <memory>

class BoxCollision: public ShapeCollision
{
public:
	Vec3 halfExtends;

	BoxCollision(GameObject* _gameObject, Vec3 _halfExtends = { 1.0f, 1.0f, 1.0f }, bool _isTrigger = false, Transform _tranform = Transform());
	BoxCollision(const BoxCollision&);
	Component* Clone() const override { return new BoxCollision(*this); }
	~BoxCollision();

	void Editor() override;
	void DrawGizmos(const Camera& _camera) override;
	void UpdateScale() override;

	const char* GetComponentName() override { return "BoxCollision"; }

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(halfExtends.x, halfExtends.y, halfExtends.z);
		_ar(isTrigger);
		_ar(transform.localPosition.x, transform.localPosition.y, transform.localPosition.z,
			transform.localRotation.x, transform.localRotation.y, transform.localRotation.z, transform.localRotation.w,
			transform.localScale.x, transform.localScale.y, transform.localScale.z);
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<BoxCollision>& _construct)
	{
		Vec3 halfExtends;
		bool trigger;
		Transform t;
		_ar(halfExtends.x, halfExtends.y, halfExtends.z);
		_ar(trigger);
		_ar(t.localPosition.x, t.localPosition.y, t.localPosition.z,
			t.localRotation.x, t.localRotation.y, t.localRotation.z, t.localRotation.w,
			t.localScale.x, t.localScale.y, t.localScale.y);

		_construct(GameObject::currentLoadedGameObject, halfExtends, trigger, t);
		_ar(cereal::base_class<Component>(_construct.ptr()));
	}
};

CEREAL_REGISTER_TYPE(BoxCollision)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ShapeCollision, BoxCollision)