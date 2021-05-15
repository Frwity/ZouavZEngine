#pragma once

#include "Component/Component.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"

namespace physx
{
	class PxGeometry;
	class PxRigidDynamic;
}

class RigidBody : public Component
{
private:
	void AttachShape();
protected:
	void InternalActivate() override;
	void InternalDehactivate() override;
public:
	physx::PxRigidDynamic* actor = nullptr;

	RigidBody() = delete;
	RigidBody(GameObject* _gameObject);
	RigidBody(const RigidBody&) = default;
	Component* Clone() const override { return new RigidBody(*this); }
	~RigidBody();

	void Editor() override;

	const char* GetComponentName() override { return "RigidBody"; }

	void SetLinearVelocity(const class Vec3& v);
	class Vec3 GetLinearVelocity();
	void DisableGravity();
	void EnableGravity();
	void OnContact(GameObject* _other);
	void OnTrigger(GameObject* _other);

	void Activate() override;
	void Dehactivate() override;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<RigidBody>& _construct);
};

CEREAL_REGISTER_TYPE(RigidBody)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, RigidBody)
