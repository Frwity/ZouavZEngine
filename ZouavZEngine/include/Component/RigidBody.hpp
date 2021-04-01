#pragma once

#include "Component/Component.hpp"

namespace physx
{
	class PxGeometry;
	class PxRigidDynamic;
}

class RigidBody: public Component
{
public:
	physx::PxRigidDynamic* actor = nullptr;

	RigidBody(GameObject* _gameObject);
	~RigidBody();

	void SetLinearVelocity(class Vec3& v);
	class Vec3 GetLinearVelocity();
	void DisableGravity();
	void EnableGravity();

private:
	void AttachShape();

};