#pragma once

#include "Component/Component.hpp"
#include "Transform.hpp"

namespace physx
{
	class PxRigidActor;
	class PxShape;
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
	Rigid(class GameObject* _gameObject, std::string _name = "Rigid");
	~Rigid();

	void Activate() override;
	void Dehactivate() override;

	virtual void UpdateTransform(Transform transform) {};

	virtual void OnContact(GameObject* _other, physx::PxShape* _collidingShap);
	virtual void OnTrigger(GameObject* _other, physx::PxShape* _collidingShape);
};