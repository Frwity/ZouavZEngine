#pragma once

#include "Component/Component.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/archives/json.hpp"

class ScriptComponent: public Component
{
private:

public:
	ScriptComponent() = delete;
	ScriptComponent(class GameObject* _gameObject);
	virtual ~ScriptComponent();

	const char* GetComponentName() override { return "ScriptComponent"; }

	virtual void Begin() {};
	virtual void FixedUpdate() {};
	virtual void Update() {};
	virtual void OnDestroy() {};
	virtual void OnTrigger(class GameObject* _other, class ShapeCollision* _triggerShape) {};
	virtual void OnContact(class GameObject* _other, class ShapeCollision* _triggerShape) {};

	template <class Archive>
	void serialize(Archive& _ar)
	{
		_ar(cereal::base_class<Component>(this));
	}

	template <class Archive>
	static void load_and_construct(Archive& _ar, cereal::construct<ScriptComponent>& _construct)
	{
		_ar(cereal::base_class<Component>(_construct.ptr()));
	}
};

CEREAL_REGISTER_TYPE(ScriptComponent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ScriptComponent)