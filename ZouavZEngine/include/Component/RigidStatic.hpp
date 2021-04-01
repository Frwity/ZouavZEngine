#pragma once
#include "Component/Component.hpp"

namespace physx
{
	class PxRigidStatic;
}

class RigidStatic: public Component
{
public:
	physx::PxRigidStatic* actor = nullptr;

	RigidStatic(GameObject* _gameObject);
	~RigidStatic();

private:
	void AttachShape();
};