#pragma once
#include "Component/Component.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"

namespace physx
{
	class PxRigidStatic;
}
#include <iostream>

class RigidStatic: public Component
{
private:
	void AttachShape();
public:
	physx::PxRigidStatic* actor = nullptr;

	RigidStatic() = delete;
	RigidStatic(GameObject* _gameObject);
	~RigidStatic();

	void Editor() override;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		
	}
	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<RigidStatic>& _construct)
	{
		_construct(GameObject::currentLoadedGameObject);
	}
};


CEREAL_REGISTER_TYPE(RigidStatic)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, RigidStatic)