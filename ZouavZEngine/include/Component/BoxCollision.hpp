#pragma once

#include "Component/ShapeCollision.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include "Rendering/Mesh.hpp"
#include <memory>

class BoxCollision: public ShapeCollision
{
public:
	Vec3 halfExtends;
	std::shared_ptr<Mesh> cube;

	BoxCollision(GameObject* _gameObject, Vec3 _halfExtends = { 0.5f, 0.5f, 2.0f }, bool _isTrigger = false, Transform _tranform = Transform());
	BoxCollision(const BoxCollision&);
	Component* Clone() const override { return new BoxCollision(*this); }
	~BoxCollision();

	void Editor() override;
	void UpdateExtends();
	void DrawGizmos(const Camera& _camera) override;

	const char* GetComponentName() override { return "BoxCollision"; }

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(cereal::base_class<Component>(this));
		_ar(halfExtends.x, halfExtends.y, halfExtends.z);
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<BoxCollision>& _construct)
	{
		Vec3 halfExtends;
		_ar(halfExtends.x, halfExtends.y, halfExtends.z);
		_construct(GameObject::currentLoadedGameObject, halfExtends);
		_ar(cereal::base_class<Component>(_construct.ptr()));
	}
};

CEREAL_REGISTER_TYPE(BoxCollision)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ShapeCollision, BoxCollision)