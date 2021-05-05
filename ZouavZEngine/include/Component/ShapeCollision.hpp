#pragma once
#include "Component.hpp"
#include "PxSimulationEventCallback.h"
#include "System/Debug.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include "GameObject.hpp"

namespace physx
{
	class PxActor;
	class PxMaterial;
}

class ShapeCollision: public Component
{
protected:
	physx::PxMaterial* material = nullptr;
	void AttachToRigidComponent();
	class Transform transform;
public:
	physx::PxShape* shape = nullptr;
	bool isAttach = false;
	bool isTrigger = false;

	ShapeCollision(GameObject* _gameObject/*, class Transform transform*/, bool _isTrigger = false);
	~ShapeCollision();

	const char* GetComponentName() override { return "ShapeCollision"; }
	void releasePhysXComponent();
	void UpdateIsTrigger();
	void UpdateTransform(class Transform& _transform);
		
	template <class Archive>
	void serialize(Archive& _ar)
	{
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<ShapeCollision>& _construct)
	{
		_construct(GameObject::currentLoadedGameObject);
	}

};

CEREAL_REGISTER_TYPE(ShapeCollision)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ShapeCollision)