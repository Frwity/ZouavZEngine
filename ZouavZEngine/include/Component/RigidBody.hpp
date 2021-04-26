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

public:
	physx::PxRigidDynamic* actor = nullptr;

	RigidBody() = delete;
	RigidBody(GameObject* _gameObject);
	~RigidBody();

	void Editor() override;

	const char* GetComponentName() override { return "RigidBody"; }

	void SetLinearVelocity(const class Vec3& v);
	class Vec3 GetLinearVelocity();
	void DisableGravity();
	void EnableGravity();
	void OnContact(GameObject* _other);
	void OnTrigger(GameObject* _other);


	template <class Archive>
	void serialize(Archive& _ar)
	{
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<RigidBody>& _construct);
};

CEREAL_REGISTER_TYPE(RigidBody)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, RigidBody)
