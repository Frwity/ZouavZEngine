#pragma once

#include "Component/ShapeCollision.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include "Rendering/Mesh.hpp"
#include <memory>

class CapsuleCollision: public ShapeCollision
{
private:
	std::shared_ptr<Mesh> capsule;
public:
	float radius;
	float halfHeight;

	CapsuleCollision(GameObject* _gameObject, float _radius = 1.0f, float _halfHeight = 1.0f, bool _isTrigger = false, Transform _tranform = Transform());
	CapsuleCollision(const CapsuleCollision&);
	Component* Clone() const override { return new CapsuleCollision(*this); }
	~CapsuleCollision();

	void Editor() override;
	void UpdateCapsule();
	void DrawGizmos(const Camera& _camera) override;

	const char* GetComponentName() override { return "CapsuleCollision"; }

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(radius, halfHeight);
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<CapsuleCollision>& _construct)
	{
		float _radius;
		float _halfHeight;
		_ar(_radius, _halfHeight);
		_construct(GameObject::currentLoadedGameObject, _radius, _halfHeight);
		_ar(cereal::base_class<Component>(_construct.ptr()));
	}
};

CEREAL_REGISTER_TYPE(CapsuleCollision)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ShapeCollision, CapsuleCollision)