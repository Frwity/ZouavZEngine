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
protected:

	void InternalActivate() override;
	void InternalDehactivate() override;
public:
	physx::PxRigidStatic* actor = nullptr;

	RigidStatic() = delete;
	RigidStatic(GameObject* _gameObject);
	RigidStatic(const RigidStatic&);
	Component* Clone() const override { return new RigidStatic(*this); }
	~RigidStatic();

	void Editor() override;

	const char* GetComponentName() override { return "RigidStatic"; }

	void Activate() override;
	void Dehactivate() override;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(cereal::base_class<Component>(this));
	}
	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<RigidStatic>& _construct);
};


CEREAL_REGISTER_TYPE(RigidStatic)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, RigidStatic)