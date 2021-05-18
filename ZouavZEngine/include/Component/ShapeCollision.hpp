#pragma once
#include "Component.hpp"
#include "PxSimulationEventCallback.h"
#include "System/Debug.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"
#include "Rendering/Shader.hpp"
#include "Rendering/Mesh.hpp"
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
	Transform transform;
	std::shared_ptr<Shader> gizmoShader;
	std::shared_ptr<Mesh> gizmoMesh;
	void AttachToRigidComponent();
	void InternalActivate() override;
	void InternalDehactivate() override;
public:
	physx::PxShape* shape = nullptr;
	bool isAttach = false;
	bool isTrigger = false;

	ShapeCollision(GameObject* _gameObject, Transform _transform = Transform(), bool _isTrigger = false);
	ShapeCollision(const ShapeCollision&);
	Component* Clone() const override { return new ShapeCollision(*this); }
	~ShapeCollision();

	const char* GetComponentName() override { return "ShapeCollision"; }
	void releasePhysXComponent();
	void UpdateIsTrigger();
	void UpdateTransform(Transform _transform);
	virtual void Editor() override;
	virtual void DrawGizmos(const Camera& _camera, const Mat4& _modelMatrix = Mat4::identity);

	void Activate() override;
	void Dehactivate() override;

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<ShapeCollision>& _construct)
	{
		_ar(cereal::base_class<Component>(_construct.ptr()));
	}

};

CEREAL_REGISTER_TYPE(ShapeCollision)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ShapeCollision)