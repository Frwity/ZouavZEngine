#pragma once

#include "Component/Component.hpp"
#include "Transform.hpp"

namespace physx
{
	class PxRigidActor;
}

class Rigid : public Component
{
protected:
	void AttachShape();
	void InternalActivate() override;
	void InternalDehactivate() override;
public:
	physx::PxRigidActor* actor = nullptr;

	Rigid() = delete;
	Rigid(class GameObject* _gameObject);
	Rigid(const Rigid&);
	Component* Clone() const override { return new Rigid(*this); }
	~Rigid();

	const char* GetComponentName() override { return "Rigid"; }

	void Activate() override;
	void Dehactivate() override;

	virtual void UpdateTransform(Transform transform) {};

	virtual void OnContact(GameObject* _other);
	virtual void OnTrigger(GameObject* _other);
};