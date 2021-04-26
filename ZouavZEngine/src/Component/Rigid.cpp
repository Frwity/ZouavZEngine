#include "Component/Rigid.hpp"
#include "GameObject.hpp"
#include "PxRigidBody.h"
#include "PxRigidDynamic.h"
#include "PxShape.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "extensions/PxSimpleFactory.h"
#include "extensions/PxRigidBodyExt.h"
#include "System/PhysicUtils.hpp"
#include "Component/ShapeCollision.hpp"
#include "Component/ScriptComponent.hpp"

Rigid::Rigid(GameObject* _gameObject)
	: Component(_gameObject)
{

}

void Rigid::AttachShape()
{
	//TODO use getComponents
	ShapeCollision* collision = GetGameObject().GetComponent<ShapeCollision>();

	if (collision && !collision->isAttach)
	{
		collision->isAttach = true;
		if (!actor->attachShape(*collision->shape))
			std::cout << "Attach to shape failed !" << std::endl;
		//collision->shape->release();
	}
}

void Rigid::OnContact(GameObject* _other)
{
	if (gameObject == nullptr)
		return;

	ScriptComponent* script = GetGameObject().GetComponent<ScriptComponent>();

	if (script)
		script->OnContact(_other);
}

void Rigid::OnTrigger(GameObject* _other)
{
	ScriptComponent* script = GetGameObject().GetComponent<ScriptComponent>();

	if (script)
		script->OnTrigger(_other);
}