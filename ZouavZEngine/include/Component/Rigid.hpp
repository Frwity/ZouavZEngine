#pragma once

#include "Component/Component.hpp"

namespace physx
{
	class PxRigidActor;
}

class Rigid : public Component
{
protected:
	void AttachShape();

public:
	physx::PxRigidActor* actor = nullptr;

	Rigid() = delete;
	Rigid(class GameObject* _gameObject);

	virtual void OnContact(GameObject* _other);
	virtual void OnTrigger(GameObject* _other);
};