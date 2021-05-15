#pragma once
#include "Component.hpp"
#include "PxSimulationEventCallback.h"
#include "System/Debug.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include "Rendering/Material.hpp"
#include "Rendering/Camera.hpp"
#include "GameObject.hpp"

namespace physx
{
	class PxActor;
	class PxMaterial;
}

class ShapeCollision: public Component
{
protected:
	physx::PxMaterial* material = nullptr;
	void AttachToRigidComponent();
	Transform transform;
	Material materialShader;
public:
	physx::PxShape* shape = nullptr;
	bool isAttach = false;
	bool isTrigger = false;

	ShapeCollision(GameObject* _gameObject, Transform _transform = Transform(), bool _isTrigger = false);
	~ShapeCollision();

	const char* GetComponentName() override { return "ShapeCollision"; }
	void releasePhysXComponent();
	void UpdateIsTrigger();
	void UpdateTransform(Transform _transform);
	virtual void Editor() override;
	virtual void DrawGizmos(const Camera& _camera);
		
	template <class Archive>
	void serialize(Archive& _ar)
	{
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<ShapeCollision>& _construct)
	{
		_construct(GameObject::currentLoadedGameObject);
	}

};

CEREAL_REGISTER_TYPE(ShapeCollision)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ShapeCollision)