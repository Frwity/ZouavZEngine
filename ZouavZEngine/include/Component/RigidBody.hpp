#pragma once

#include "Component/Component.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"

namespace physx
{
	class PxGeometry;
	class PxRigidDynamic;
}

class RigidBody: public Component
{
private:
	void AttachShape();

public:
	physx::PxRigidDynamic* actor = nullptr;

	RigidBody() = delete;
	RigidBody(GameObject* _gameObject);
	~RigidBody();

	void SetLinearVelocity(class Vec3& v);
	class Vec3 GetLinearVelocity();
	void DisableGravity();
	void EnableGravity();


	template <class Archive>
	void serialize(Archive& _ar)
	{
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<RigidBody>& _construct)
	{
		_construct(GameObject::currentLoadedGameObject);
	}
};

CEREAL_REGISTER_TYPE(RigidBody)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, RigidBody)