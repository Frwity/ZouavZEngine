#pragma once

#include "Component/ShapeCollision.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include "Rendering/Mesh.hpp"
#include <memory>

class SphereCollision : public ShapeCollision
{
public:
	float radius;
	std::shared_ptr<Mesh> sphereMesh;

	SphereCollision(GameObject* _gameObject, float _radius = 0.5f, bool _isTrigger = false, Transform _tranform = Transform());
	SphereCollision(const SphereCollision&);
	Component* Clone() const override { return new SphereCollision(*this); }
	~SphereCollision();

	void Editor() override;
	void UpdateScale();
	void DrawGizmos(const Camera& _camera) override;

	const char* GetComponentName() override { return "SphereCollision"; }

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(radius);
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<SphereCollision>& _construct)
	{
		float _radius;
		_ar(_radius);
		_construct(GameObject::currentLoadedGameObject, _radius);
		_ar(cereal::base_class<Component>(_construct.ptr()));
	}
};

CEREAL_REGISTER_TYPE(SphereCollision)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ShapeCollision, SphereCollision)