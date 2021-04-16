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
public:
	physx::PxShape* shape = nullptr;
	bool isAttach = false;

	ShapeCollision(GameObject* _gameObject);
	~ShapeCollision();

	void releasePhysXComponent();
		
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