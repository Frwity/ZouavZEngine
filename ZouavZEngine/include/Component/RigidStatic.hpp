#pragma once
#include "Component/Component.hpp"
#include "Component/Rigid.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"

namespace physx
{
	class PxRigidStatic;
}
#include <iostream>

class RigidStatic: public Rigid
{
public:

	RigidStatic() = delete;
	RigidStatic(GameObject* _gameObject);
	~RigidStatic();

	void Editor() override;

	const char* GetComponentName() override { return "RigidStatic"; }

	void UpdateTransform(Transform transform) override;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		
	}
	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<RigidStatic>& _construct);
};


CEREAL_REGISTER_TYPE(RigidStatic)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, RigidStatic)